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
/** \addtogroup cc2538
 * @{
 *
 * \defgroup cc2538-smartrf SmartRF06EB Peripherals
 *
 * Defines related to the SmartRF06EB
 *
 * This file provides connectivity information on LEDs, Buttons, UART and
 * other SmartRF peripherals
 *
 * Notably, PC0 is used to drive LED1 as well as the USB D+ pullup. Therefore
 * when USB is enabled, LED1 can not be driven by firmware.
 *
 * This file can be used as the basis to configure other platforms using the
 * cc2538 SoC.
 * @{
 *
 * \file
 * Header file with definitions related to the I/O connections on the TI
 * SmartRF06EB
 *
 * \note   Do not include this file directly. It gets included by contiki-conf
 *         after all relevant directives have been set.
 */
#ifndef BOARD_H_
#define BOARD_H_

//#include "dev/gpio.h"
#include "lib/_gpio.h"
#include "dev/nvic.h"
#include "lib/hw_memmap.h"
/*---------------------------------------------------------------------------*/
/** \name SmartRF LED configuration
 *
 * LEDs on the SmartRF06 (EB and BB) are connected as follows:
 * - LED1 (Red)    -> PC0
 * - LED2 (Yellow) -> PC1
 * - LED3 (Green)  -> PC2
 * - LED4 (Orange) -> PC3
 *
 * LED1 shares the same pin with the USB pullup
 * @{
 */
/*---------------------------------------------------------------------------*/
/* Some files include leds.h before us, so we need to get rid of defaults in
 * leds.h before we provide correct definitions */
#undef LEDS_GREEN
#undef LEDS_YELLOW
#undef LEDS_RED
#undef LEDS_CONF_ALL

#define LEDS_YELLOW    32   /**< LED2 (Yellow) -> PC5 */
#define LEDS_GREEN     64   /**< LED3 (Green)  -> PC6 */
#define LEDS_ORANGE    128  /**< LED4 (Orange) -> PC7 */

#if USB_SERIAL_CONF_ENABLE
#define LEDS_CONF_ALL 224
#define LEDS_RED LEDS_ORANGE
#else
#define LEDS_CONF_ALL 224
#define LEDS_RED       1 /**< LED1 (Red)    -> PC0 */
#endif


/* Notify various examples that we have LEDs */
#define PLATFORM_HAS_LEDS        1
/** @} */
/*---------------------------------------------------------------------------*/
/** \name USB configuration
 *
 * The USB pullup is driven by PC0 and is shared with LED1
 */
#define USB_PULLUP_PORT          GPIO_C_BASE
#define USB_PULLUP_PIN           0
#define USB_PULLUP_PIN_MASK      (1 << USB_PULLUP_PIN)
/** @} */
/*---------------------------------------------------------------------------*/
/** \name UART configuration
 *
 * On the SmartRF06EB, the UART (XDS back channel) is connected to the
 * following ports/pins
 * - RX:  PA0
 * - TX:  PA1
 * - CTS: PB0 (Can only be used with UART1)
 * - RTS: PD3 (Can only be used with UART1)
 *
 * We configure the port to use UART0. To use UART1, change UART_CONF_BASE
 * @{
 */
#define UART_CONF_BASE           UART_0_BASE

#define UART_RX_PORT             GPIO_A_NUM
#define UART_RX_PIN              0

#define UART_TX_PORT             GPIO_A_NUM
#define UART_TX_PIN              1

#define UART_CTS_PORT            GPIO_B_NUM
#define UART_CTS_PIN             0

#define UART_RTS_PORT            GPIO_D_NUM
#define UART_RTS_PIN             3
/** @} */
/*---------------------------------------------------------------------------*/
/** \name SmartRF Button configuration
 *
 * Buttons on the SmartRF06 are connected as follows:
 * - BUTTON_SELECT -> PA3
 * - BUTTON_LEFT -> PC4
 * - BUTTON_RIGHT -> PC5
 * - BUTTON_UP -> PC6
 * - BUTTON_DOWN -> PC7
 * @{
 */
/** BUTTON_SELECT -> PA3 */
#define BUTTON_SELECT_PORT_NO    GPIO_A_NUM
#define BUTTON_SELECT_PIN        3
#define BUTTON_SELECT_PORT       GPIO_A_BASE
#define BUTTON_SELECT_PIN_MASK   (1 << BUTTON_SELECT_PIN)
#define BUTTON_SELECT_VECTOR     NVIC_INT_GPIO_PORT_A

/** BUTTON_LEFT -> PC4 */
#define BUTTON_LEFT_PORT_NO      GPIO_C_NUM
#define BUTTON_LEFT_PIN          4
#define BUTTON_LEFT_PORT         GPIO_C_BASE
#define BUTTON_LEFT_PIN_MASK     (1 << BUTTON_LEFT_PIN)
#define BUTTON_LEFT_VECTOR       NVIC_INT_GPIO_PORT_C

/** BUTTON_RIGHT -> PC5 */
#define BUTTON_RIGHT_PORT_NO     GPIO_C_NUM
#define BUTTON_RIGHT_PIN         5
#define BUTTON_RIGHT_PORT        GPIO_C_BASE
#define BUTTON_RIGHT_PIN_MASK    (1 << BUTTON_RIGHT_PIN)
#define BUTTON_RIGHT_VECTOR      NVIC_INT_GPIO_PORT_C

/** BUTTON_UP -> PC6 */
#define BUTTON_UP_PORT_NO        GPIO_C_NUM
#define BUTTON_UP_PIN            6
#define BUTTON_UP_PORT           GPIO_C_BASE
#define BUTTON_UP_PIN_MASK       (1 << BUTTON_UP_PIN)
#define BUTTON_UP_VECTOR         NVIC_INT_GPIO_PORT_C

/** BUTTON_DOWN -> PC7 */
#define BUTTON_DOWN_PORT_NO      GPIO_C_NUM
#define BUTTON_DOWN_PIN          7
#define BUTTON_DOWN_PORT         GPIO_C_BASE
#define BUTTON_DOWN_PIN_MASK     (1 << BUTTON_DOWN_PIN)
#define BUTTON_DOWN_VECTOR       NVIC_INT_GPIO_PORT_C

/* Notify various examples that we have Buttons */
//#define PLATFORM_HAS_BUTTON      1
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name SPI configuration
 *
 * These values configure which CC2538 pins to use for the SPI lines.
 * @{
 */
#define SSI0_SPI_PORT	        GPIO_B_BASE
#define SSI0_SPI_CLK_PIN     	GPIO_PIN_3
#define SSI0_SPI_MOSI_PIN       GPIO_PIN_4
#define SSI0_SPI_MISO_PIN       GPIO_PIN_5

#define SSI0_SPI_CE0_PORT		GPIO_B_BASE
#define SSI0_SPI_CE0_PIN   		GPIO_PIN_0
#define SSI0_SPI_CE1_PORT   	GPIO_B_BASE
#define SSI0_SPI_CE1_PIN      	GPIO_PIN_1
#define SSI0_SPI_CE2_PORT   	GPIO_B_BASE
#define SSI0_SPI_CE2_PIN      	GPIO_PIN_2
#define RTC_SPI_CE_PORT   		GPIO_B_BASE
#define RTC_SPI_CE_PIN        	GPIO_PIN_6
#define NOR_FLASH_SPI_CE_PORT   GPIO_B_BASE
#define NOR_FLASH_CE_PIN        SSI0_SPI_CE0_PIN

#define CE0 			0
#define CE1 			1
#define CE2 			2


/** @} */
/*---------------------------------------------------------------------------*/

/**
 * \name I2C configuration
 *
 * These values configure which CC2538 pins to use for the I2C lines.
 * @{
 */
#define I2C_SCL_PORT             GPIO_D_BASE
#define I2C_SCL_PIN              GPIO_PIN_4
#define I2C_SDA_PORT             GPIO_D_BASE
#define I2C_SDA_PIN              GPIO_PIN_5
/** @} */
/*---------------------------------------------------------------------------*/

/**
 * \name Interrupt pin configuration
 *
 * These values configure which CC2538 pins to use for Interrupts lines.
 * @{
 */
#define INT0			    0
#define INT1			    1
#define INT2			    2
#define INT_RTC			    3

#define INT_PORT		    GPIO_D_BASE
#define INT_GPIO                    INT_GPIOD
#define INT0_PIN		    GPIO_PIN_0
#define INT1_PIN		    GPIO_PIN_1
#define INT2_PIN		    GPIO_PIN_2
#define RTC_INT_PIN		    GPIO_PIN_3
/** @} */
/*---------------------------------------------------------------------------*/

/**
 * \name DigitalIO pin configuration
 *
 * These values configure which CC2538 pins to use for DigitalIO lines.
 * @{
 */
#define DIO0			    0
#define DIO1			    1
#define DIO2			    2
#define DIO3		    	3

#define DIO_PORT			GPIO_C_BASE
#define DIO0_PIN		    GPIO_PIN_0
#define DIO1_PIN		    GPIO_PIN_1
#define DIO2_PIN		    GPIO_PIN_2
#define DIO3_PIN		    GPIO_PIN_3
/** @} */
/*---------------------------------------------------------------------------*/

/**
 * \name ADC pin configuration
 *
 * These values configure which CC2538 pins to use for ADC lines.
 * @{
 */
#define ADC0			    0
#define ADC1			    1
#define ADC2			    2
#define ADCVDD		    	3
#define ADCTEMP		    	4

#define ADC_PORT		    GPIO_A_BASE
#define ADC0_PIN		    GPIO_PIN_3
#define ADC1_PIN		    GPIO_PIN_6
#define ADC2_PIN		    GPIO_PIN_7

#define ADC0_CH			    SOCADC_AIN3
#define ADC1_CH			    SOCADC_AIN6
#define ADC2_CH			    SOCADC_AIN7
#define ADCVDD_CH		    SOCADC_VDD
#define ADCTEMP_CH		    SOCADC_TEMP_SENS

#define ADC_INTREF		    0
#define ADC_VDDREF		    1

#define ADC_INT_REF			1210	// Internal reference in volt for make the conversion

/** @} */
/*---------------------------------------------------------------------------*/

/**
 * \name Device string used on startup
 * @{
 */
#define BOARD_STRING "VirtualSense_CN_32"
/** @} */


/* LELE Init definition of VirtualSense Board (for now copied from old platform */
#define PLATFORM_HAS_EEPROM
#define PLATFORM_HAS_RTC_PCF2123
#define BASE_EEPROM     0


/* Interrupt port definitions */
//#define INT_PORT_DIR 			P2DIR
//#define INT_PORT_REN			P2REN
//#define INT_PORT_IE				P2IE
//#define INT_PORT_IES 			P2IES
//#define INT_PORT_IFG			P2IFG

//#define INT_P0BIT				BIT0
//#define INT_P1BIT				BIT3
//#define INT_P2BIT				BIT5
//#define INT_P3BIT				BIT6
//#define INT_P4BIT				BIT7
//#define INT_PRTCBIT			    BIT4

//#define INT_P0					0
//#define INT_P1					1
//#define INT_P2					2
//#define INT_P3					3
//#define INT_P4					4
//#define INT_PRTC			    5

/* digital I/O port definitions */
//#define DIO_PORT_0_DIR 			P6DIR
//#define DIO_PORT_1_DIR 			P6DIR
//#define DIO_PORT_2_DIR 			P6DIR
//#define DIO_PORT_3_DIR 			P6DIR

//#define DIO_PORT_0_IN 			P6IN
//#define DIO_PORT_1_IN 			P6IN
//#define DIO_PORT_2_IN 			P6IN
//#define DIO_PORT_3_IN 			P6IN

//#define DIO_PORT_0_OUT 			P6OUT
//#define DIO_PORT_1_OUT 			P6OUT
//#define DIO_PORT_2_OUT 			P6OUT
//#define DIO_PORT_3_OUT 			P6OUT

//#define DIO_P0BIT				BIT0
//#define DIO_P1BIT				BIT2
//#define DIO_P2BIT				BIT3
//#define DIO_P3BIT				BIT4

//#define DIO_P0					0
//#define DIO_P1					1
//#define DIO_P2					2
//#define DIO_P3					3


/* MSP430F54xx family symbols */
#define SYSCLK_1MHZ             1
#define SYSCLK_4MHZ             4
#define SYSCLK_8MHZ             8
#define SYSCLK_12MHZ            12
#define SYSCLK_16MHZ            16
#define SYSCLK_20MHZ            20
#define SYSCLK_25MHZ            25

#define DCO_MULT_1MHZ           30
#define DCO_MULT_4MHZ           122
#define DCO_MULT_8MHZ           244
#define DCO_MULT_12MHZ          366
#define DCO_MULT_16MHZ          488
#define DCO_MULT_20MHZ          610
#define DCO_MULT_25MHZ          763

#define DCORSEL_1MHZ            DCORSEL_2
#define DCORSEL_4MHZ            DCORSEL_3
#define DCORSEL_8MHZ            DCORSEL_4
#define DCORSEL_12MHZ           DCORSEL_5
#define DCORSEL_16MHZ           DCORSEL_5
#define DCORSEL_20MHZ           DCORSEL_6
#define DCORSEL_25MHZ           DCORSEL_6

#define VCORE_1MHZ              PMMCOREV_0
#define VCORE_4MHZ              PMMCOREV_0
#define VCORE_8MHZ              PMMCOREV_0
#define VCORE_12MHZ             PMMCOREV_1
#define VCORE_16MHZ             PMMCOREV_2
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

/* end MSP430F54xx family symbols */

#define ROM_ERASE_UNIT_SIZE  128
#define EEPROM_ERASE_UNIT_SIZE (128)


#define CFS_CONF_OFFSET_TYPE    long


/* Use the first 128 byte (first page) for node configuration or 1 bank of CPU flash*/
#define NODE_ID_EEPROM_OFFSET     (BASE_EEPROM + 0 * EEPROM_ERASE_UNIT_SIZE)

/* Use second 32k of external flash or bank 1 of CPU  flash for system needs (hibernation) */
#define SYSTEM_EEPROM_FS_BASE  (BASE_EEPROM + 1 * EEPROM_ERASE_UNIT_SIZE)
#define SYSTEM_EEPROM_FS_SIZE (EEPROM_ERASE_UNIT_SIZE * 64)

#define APPS_EEPROM_FS_BASE  (SYSTEM_EEPROM_FS_BASE + SYSTEM_EEPROM_FS_SIZE)



/* Use the bank 2 of CPU (min 23K) flash for applications storage. starting at 0x20000*/
#define FLASH_SEGMENT_SIZE		2048
#define APP_NODES_TABLE_BASE   (0x27F7D4) // the first segment is used to store the app table
#define APPS_FLASH_FS_BASE   (APP_NODES_TABLE_BASE + 1 * FLASH_SEGMENT_SIZE)



/* END LELE Init definition of VirtualSense Board (for now copied from old platform */

#endif /* BOARD_H_ */

/**
 * @}
 * @}
 */
