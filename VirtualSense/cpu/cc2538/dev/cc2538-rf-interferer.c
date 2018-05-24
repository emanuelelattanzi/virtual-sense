/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup cc2538-rf
 * @{
 *
 * \file
 * Implementation of the cc2538 RF driver used to generate a conmtrollable
 * interference on the network.
 *
 * By: Emanuele Lattanzi (15-05-2018)
 */
#include "contiki.h"
#include "dev/radio.h"
#include "sys/clock.h"
#include "sys/rtimer.h"
#include "net/packetbuf.h"
#include "net/rime/rimestats.h"
#include "net/rime/rimeaddr.h"
#include "net/netstack.h"
#include "sys/energest.h"
#include "dev/cc2538-rf.h"
#include "dev/gpio.h"
#include "dev/rfcore.h"
#include "dev/sys-ctrl.h"
#include "dev/udma.h"
#include "reg.h"

#include <string.h>
/*---------------------------------------------------------------------------*/

/* INTERFERER CONFIGURATION PARAMTERS
 *
 *
 *  ON_TIME   is the time during witch the transmitter stay on
 *  OFF_TIME  is the time during witch the transmitter stay off
 *
 *  unmodulated MDMTEST1 0x18
 *  modulated   MDMTEST1 0x08
 *
 *  FSCAL1           0x00 2 W
 *  AGCCTRL1         0x15 2 W
 *  TXFILTCFG        0x09 2 W
 *  MDMTEST1         0x08 2 W
 *  FSCAL1           0x01 2 W
 *  FRMCTRL0         0x43 2 W
 *  FRMCTRL1         0x00 2 W
 *  FRMFILT0         0x0d 2 W
 *  GPIO_C_DIR       0x0c 2 W
 *  GPIO_C_DATA      0x00 2 W
 *  GPIO_D_DIR       0x04 2 W
 *  GPIO_D_DATA      0x00 2 W
 *  MDMTEST1         0x08 2 W
 *
 *  */

#define UNMODULATED 0
#define ONTIME RTIMER_ARCH_SECOND/1000


#define CHECKSUM_LEN 2

/* uDMA channel control persistent flags */
#define UDMA_TX_FLAGS (UDMA_CHCTL_ARBSIZE_128 | UDMA_CHCTL_XFERMODE_AUTO \
    | UDMA_CHCTL_SRCSIZE_8 | UDMA_CHCTL_DSTSIZE_8 \
    | UDMA_CHCTL_SRCINC_8 | UDMA_CHCTL_DSTINC_NONE)

#define UDMA_RX_FLAGS (UDMA_CHCTL_ARBSIZE_128 | UDMA_CHCTL_XFERMODE_AUTO \
    | UDMA_CHCTL_SRCSIZE_8 | UDMA_CHCTL_DSTSIZE_8 \
    | UDMA_CHCTL_SRCINC_NONE | UDMA_CHCTL_DSTINC_8)

/*
 * uDMA transfer threshold. DMA will only be used to read an incoming frame
 * if its size is above this threshold
 */
#define UDMA_RX_SIZE_THRESHOLD 3
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
/*---------------------------------------------------------------------------*/
/* Local RF Flags */
#define RX_ACTIVE     0x80
#define RF_MUST_RESET 0x40
#define WAS_OFF       0x10
#define RF_ON         0x01

/* Bit Masks for the last byte in the RX FIFO */
#define CRC_BIT_MASK 0x80
#define LQI_BIT_MASK 0x7F
/* RSSI Offset */
#define RSSI_OFFSET    73

/* 192 usec off -> on interval (RX Callib -> SFD Wait). We wait a bit more */
#define ONOFF_TIME                    RTIMER_ARCH_SECOND / 3125
/*---------------------------------------------------------------------------*/
/* Sniffer configuration */
#ifndef CC2538_RF_CONF_SNIFFER_USB
#define CC2538_RF_CONF_SNIFFER_USB 0
#endif

#if CC2538_RF_CONF_SNIFFER
static const uint8_t magic[] = { 0x53, 0x6E, 0x69, 0x66 };      /** Snif */

#if CC2538_RF_CONF_SNIFFER_USB
#include "usb/usb-serial.h"
#define write_byte(b) usb_serial_writeb(b)
#define flush()       usb_serial_flush()
#else
#include "dev/uart.h"
#define write_byte(b) uart_write_byte(b)
#define flush()
#endif

#else /* CC2538_RF_CONF_SNIFFER */
#define write_byte(b)
#define flush()
#endif /* CC2538_RF_CONF_SNIFFER */
/*---------------------------------------------------------------------------*/
#ifdef CC2538_RF_CONF_AUTOACK
#define CC2538_RF_AUTOACK CC2538_RF_CONF_AUTOACK
#else
#define CC2538_RF_AUTOACK 1
#endif
/*---------------------------------------------------------------------------*/
static uint8_t rf_flags;

static int on(void);
static int off(void);
/*---------------------------------------------------------------------------*/
PROCESS(cc2538_rf_interferer_process, "cc2538 RF interferer driver");

static struct etimer et;

/*---------------------------------------------------------------------------*/
uint8_t
cc2538_rf_interferer_channel_get()
{
  uint8_t chan = REG(RFCORE_XREG_FREQCTRL) & RFCORE_XREG_FREQCTRL_FREQ;

  return ((chan - CC2538_RF_CHANNEL_MIN) / CC2538_RF_CHANNEL_SPACING
          + CC2538_RF_CHANNEL_MIN);
}
/*---------------------------------------------------------------------------*/
int8_t
cc2538_rf_interferer_channel_set(uint8_t channel)
{
  PRINTF("RF-interferer: Set Channel\n");

  if((channel < CC2538_RF_CHANNEL_MIN) || (channel > CC2538_RF_CHANNEL_MAX)) {
    return -1;
  }

  /* Changes to FREQCTRL take effect after the next recalibration */
  off();
  REG(RFCORE_XREG_FREQCTRL) = (CC2538_RF_CHANNEL_MIN
      + (channel - CC2538_RF_CHANNEL_MIN) * CC2538_RF_CHANNEL_SPACING);
  on();

  return (int8_t) channel;
}
/*---------------------------------------------------------------------------*/
uint8_t
cc2538_rf_interferer_power_set(uint8_t new_power)
{
  PRINTF("RF-interferer: Set Power\n");

  REG(RFCORE_XREG_TXPOWER) = new_power;

  return (REG(RFCORE_XREG_TXPOWER) & 0xFF);
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Netstack API radio driver functions */
/*---------------------------------------------------------------------------*/
static int
channel_clear(void)
{
  int cca = 1;

  PRINTF("RF-interferer: CCA\n");

  return cca;
}
/*---------------------------------------------------------------------------*/
static int
on(void)
{
  PRINTF("RF-interferer: On\n");

  //if(!(rf_flags & RX_ACTIVE)) {
  //  CC2538_RF_CSP_ISFLUSHRX();
    CC2538_RF_CSP_ISTXON();

   // rf_flags |= RX_ACTIVE;
  //}


  return 1;
}
/*---------------------------------------------------------------------------*/
static int
off(void)
{
  PRINTF("RF-interferer: Off\n");

      CC2538_RF_CSP_ISRFOFF();

    return 1;
}
/*---------------------------------------------------------------------------*/
static int
init(void)
{
  PRINTF("RF-interferer: Init\n");


  /* Enable clock for the RF Core while Running, in Sleep and Deep Sleep */
  REG(SYS_CTRL_RCGCRFC) = 1;
  REG(SYS_CTRL_SCGCRFC) = 1;
  REG(SYS_CTRL_DCGCRFC) = 1;

  //REG(RFCORE_XREG_CCACTRL0) = CC2538_RF_CCA_THRES_USER_GUIDE;

  /*
   * Changes from default values
   * See User Guide, section "Register Settings Update"
   */
  REG(RFCORE_XREG_FSCAL1)    = 0x00;
  REG(RFCORE_XREG_AGCCTRL1)  = 0x15;     /** AGC target value */
  REG(RFCORE_XREG_TXFILTCFG) = 0x09;    /** TX anti-aliasing filter bandwidth */
  REG(RFCORE_XREG_FSCAL1)    = 0x01;
  REG(RFCORE_XREG_FRMCTRL0)  = 0x43;
  REG(RFCORE_XREG_FRMCTRL1)  = 0x00;
  REG(RFCORE_XREG_FRMFILT0)  = 0x0d;

  /*GPIO_WRITE_PIN(GPIO_C_BASE, GPIO_DIR, 0x0c);
  GPIO_WRITE_PIN(GPIO_C_BASE, GPIO_DATA, 0x00);

  GPIO_WRITE_PIN(GPIO_D_BASE, GPIO_DIR, 0x04);
  GPIO_WRITE_PIN(GPIO_D_BASE, GPIO_DATA, 0x00);*/



  cc2538_rf_power_set(CC2538_RF_TX_POWER);
  cc2538_rf_channel_set(CC2538_RF_CHANNEL);

#if UNMODULATED
  REG(RFCORE_XREG_MDMTEST1)  = 0x18;
#else
  REG(RFCORE_XREG_MDMTEST1)  = 0x08;
#endif



  PRINTF("RF-interferer: Init Done starting main loop process\n");
  process_start(&cc2538_rf_interferer_process, NULL);

  return 1;
}
/*---------------------------------------------------------------------------*/
static int
prepare(const void *payload, unsigned short payload_len)
{
  uint8_t i;

  PRINTF("RF-interferer: Prepare 0x%02x bytes\n", payload_len + CHECKSUM_LEN);

  /*
   * When we transmit in very quick bursts, make sure previous transmission
   * is not still in progress before re-writing to the TX FIFO
   */
  while(REG(RFCORE_XREG_FSMSTAT1) & RFCORE_XREG_FSMSTAT1_TX_ACTIVE);

  if((rf_flags & RX_ACTIVE) == 0) {
    on();
  }

  CC2538_RF_CSP_ISFLUSHTX();

  PRINTF("RF-interferer: data = ");
  /* Send the phy length byte first */
  REG(RFCORE_SFR_RFDATA) = payload_len + CHECKSUM_LEN;

  if(CC2538_RF_CONF_TX_USE_DMA) {
    PRINTF("<uDMA payload>");

    /* Set the transfer source's end address */
    udma_set_channel_src(CC2538_RF_CONF_TX_DMA_CHAN,
                         (uint32_t)(payload) + payload_len - 1);

    /* Configure the control word */
    udma_set_channel_control_word(CC2538_RF_CONF_TX_DMA_CHAN,
                                  UDMA_TX_FLAGS | udma_xfer_size(payload_len));

    /* Enabled the RF TX uDMA channel */
    udma_channel_enable(CC2538_RF_CONF_TX_DMA_CHAN);

    /* Trigger the uDMA transfer */
    udma_channel_sw_request(CC2538_RF_CONF_TX_DMA_CHAN);

    /*
     * No need to wait for this to end. Even if transmit() gets called
     * immediately, the uDMA controller will stream the frame to the TX FIFO
     * faster than transmit() can empty it
     */
  } else {
    for(i = 0; i < payload_len; i++) {
      REG(RFCORE_SFR_RFDATA) = ((unsigned char *)(payload))[i];
      PRINTF("%02x", ((unsigned char *)(payload))[i]);
    }
  }
  PRINTF("\n");

  return 0;
}
/*---------------------------------------------------------------------------*/
static int
transmit(unsigned short transmit_len)
{
  uint8_t counter;
  int ret = RADIO_TX_OK;

  return ret;
}
/*---------------------------------------------------------------------------*/
static int
send(const void *payload, unsigned short payload_len)
{

  return RADIO_TX_OK;
}
/*---------------------------------------------------------------------------*/
static int
read(void *buf, unsigned short bufsize)
{


  return (0);
}
/*---------------------------------------------------------------------------*/
static int
receiving_packet(void)
{
  PRINTF("RF-interferer: Receiving\n");


}
/*---------------------------------------------------------------------------*/
static int
pending_packet(void)
{
  PRINTF("RF-interferer: Pending\n");

  return (0);
}
/*---------------------------------------------------------------------------*/
const struct radio_driver cc2538_rf_interferer_driver = {
  init,
  prepare,
  transmit,
  send,
  read,
  channel_clear,
  receiving_packet,
  pending_packet,
  on,
  off,
};
/*---------------------------------------------------------------------------*/
/**
 * \brief Implementation of the cc2538 RF driver process
 *
 *        This process is started by init(). It simply sits there waiting for
 *        an event. Upon frame reception, the RX ISR will poll this process.
 *        Subsequently, the contiki core will generate an event which will
 *        call this process so that the received frame can be picked up from
 *        the RF RX FIFO
 *
 */
PROCESS_THREAD(cc2538_rf_interferer_process, ev, data)
{
  int len;
  rtimer_clock_t t0;
  PROCESS_BEGIN();
  etimer_set(&et, CLOCK_SECOND);

  while(1) {
	  PRINTF("RF-interferer main loop\n"); /* a clock cycle corresponds to  7.81 ms */
	  	  	  	  	  	  	  	  	  	   /* the transmission time of a 110bytes packet is about 8 ms */
	  on();

	  t0 = RTIMER_NOW();
	  while(RTIMER_CLOCK_LT(RTIMER_NOW(), t0 + ONTIME*4)){}

	  /*etimer_set(&et, CLOCK_SECOND/128*1);
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));*/
      off();
      t0 = RTIMER_NOW();
      	  while(RTIMER_CLOCK_LT(RTIMER_NOW(), t0 + ONTIME*14)){}
      /*etimer_set(&et, CLOCK_SECOND/128*1);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));*/

  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * \brief The cc2538 RF RX/TX ISR
 *
 *        This is the interrupt service routine for all RF interrupts relating
 *        to RX and TX. Error conditions are handled by cc2538_rf_err_isr().
 *        Currently, we only acknowledge the FIFOP interrupt source.
 */
void
cc2538_rf_interferer_rx_tx_isr(void)
{



}
/*---------------------------------------------------------------------------*/
/**
 * \brief The cc2538 RF Error ISR
 *
 *        This is the interrupt service routine for all RF errors. We
 *        acknowledge every error type and instead of trying to be smart and
 *        act differently depending on error condition, we simply reset the
 *        transceiver. RX FIFO overflow is an exception, we ignore this error
 *        since read() handles it anyway.
 *
 *        However, we don't want to reset within this ISR. If the error occurs
 *        while we are reading a frame out of the FIFO, trashing the FIFO in
 *        the middle of read(), would result in further errors (RX underflows).
 *
 *        Instead, we set a flag and poll the driver process. The process will
 *        reset the transceiver without any undesirable consequences.
 */
void
cc2538_rf_interferer_err_isr(void)
{

}
/*---------------------------------------------------------------------------*/
void
cc2538_rf_interferer_set_promiscous_mode(char p)
{

}
/*---------------------------------------------------------------------------*/
/** @} */
