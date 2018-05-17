/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Convenience function for printing system statistics
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "lib/print-stats.h"

#include "net/rime.h"
#include "sys/energest.h"
#include "lpm.h"

#include <stdio.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#define PRINTA(FORMAT,args...) printf_P(PSTR(FORMAT),##args)
#else
#define PRINTA(...) printf(__VA_ARGS__)
#endif

/*---------------------------------------------------------------------------*/
void
print_stats(void)
{
#if RIMESTATS_CONF_ENABLED
  PRINTA("S %d.%d clock %lu tx %lu rx %lu rtx %lu rrx %lu rexmit %lu acktx %lu noacktx %lu ackrx %lu timedout %lu badackrx %lu toolong %lu tooshort %lu badsynch %lu badcrc %lu contentiondrop %lu sendingdrop %lu lltx %lu llrx %lu\n",
	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	 clock_seconds(),
	 RIMESTATS_GET(tx), RIMESTATS_GET(rx),
	 RIMESTATS_GET(reliabletx), RIMESTATS_GET(reliablerx),
	 RIMESTATS_GET(rexmit), RIMESTATS_GET(acktx), RIMESTATS_GET(noacktx),
	 RIMESTATS_GET(ackrx), RIMESTATS_GET(timedout), RIMESTATS_GET(badackrx),
	 RIMESTATS_GET(toolong), RIMESTATS_GET(tooshort),
	 RIMESTATS_GET(badsynch), RIMESTATS_GET(badcrc),
	 RIMESTATS_GET(contentiondrop), RIMESTATS_GET(sendingdrop),
	 RIMESTATS_GET(lltx), RIMESTATS_GET(llrx));
#endif /* RIMESTATS_CONF_ENABLED */
#if ENERGEST_CONF_ON
  PRINTA("E %d.%d clock %lu cpu %lu lpm %lu irq %lu gled %lu yled %lu rled %lu tx %lu listen %lu sensors %lu serial %lu\n",
	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	 clock_seconds(),
	 energest_total_time[ENERGEST_TYPE_CPU].current,
	 energest_total_time[ENERGEST_TYPE_LPM].current,
	 energest_total_time[ENERGEST_TYPE_IRQ].current,
	 energest_total_time[ENERGEST_TYPE_LED_GREEN].current,
	 energest_total_time[ENERGEST_TYPE_LED_YELLOW].current,
	 energest_total_time[ENERGEST_TYPE_LED_RED].current,
	 energest_total_time[ENERGEST_TYPE_TRANSMIT].current,
	 energest_total_time[ENERGEST_TYPE_LISTEN].current,
	 energest_total_time[ENERGEST_TYPE_SENSORS].current,
	 energest_total_time[ENERGEST_TYPE_SERIAL].current);
#endif /* ENERGEST_CONF_ON */
}
//#if LPM_CONF_STATS
  //PRINTA("LPM 0 -> %lu 1 -> %lu 2 -> %lu \n",
	 //lpm_stats[0],lpm_stats[1],lpm_stats[2]);

//#endif

void
print_packet_stats(void)
{
#if RIMESTATS_CONF_ENABLED
  PRINTA("#PACKET# %d.%d clock %lu tx %lu rx %lu rtx %lu rrx %lu rexmit %lu acktx %lu noacktx %lu ackrx %lu timedout %lu badackrx %lu toolong %lu tooshort %lu badsynch %lu badcrc %lu contentiondrop %lu sendingdrop %lu lltx %lu llrx %lu\n",
  	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
  	 clock_seconds(),
  	 RIMESTATS_GET(tx), RIMESTATS_GET(rx),
  	 RIMESTATS_GET(reliabletx), RIMESTATS_GET(reliablerx),
  	 RIMESTATS_GET(rexmit), RIMESTATS_GET(acktx), RIMESTATS_GET(noacktx),
  	 RIMESTATS_GET(ackrx), RIMESTATS_GET(timedout), RIMESTATS_GET(badackrx),
  	 RIMESTATS_GET(toolong), RIMESTATS_GET(tooshort),
  	 RIMESTATS_GET(badsynch), RIMESTATS_GET(badcrc),
  	 RIMESTATS_GET(contentiondrop), RIMESTATS_GET(sendingdrop),
  	 RIMESTATS_GET(lltx), RIMESTATS_GET(llrx));
#else
  printf("RIMESTATS_CONF_ENABLED is disabled \n");
#endif /* RIMESTATS_CONF_ENABLED */
}

void
print_energest_stats(void)
{
#if ENERGEST_CONF_ON
  PRINTA("#ENERGEST# %d.%d clock %lu cpu %lu lpm %lu irq %lu gled %lu yled %lu rled %lu tx %lu listen %lu sensors %lu serial %lu\n",
	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	 clock_seconds(),
	 energest_total_time[ENERGEST_TYPE_CPU].current,
	 energest_total_time[ENERGEST_TYPE_LPM].current,
	 energest_total_time[ENERGEST_TYPE_IRQ].current,
	 energest_total_time[ENERGEST_TYPE_LED_GREEN].current,
	 energest_total_time[ENERGEST_TYPE_LED_YELLOW].current,
	 energest_total_time[ENERGEST_TYPE_LED_RED].current,
	 energest_total_time[ENERGEST_TYPE_TRANSMIT].current,
	 energest_total_time[ENERGEST_TYPE_LISTEN].current,
	 energest_total_time[ENERGEST_TYPE_SENSORS].current,
	 energest_total_time[ENERGEST_TYPE_SERIAL].current);
#else
  printf("ENERGEST_CONF_ON is disabled \n");
#endif /* ENERGEST_CONF_ON */
}

void
print_lpm_stats(void)
{
#if LPM_CONF_STATS
  PRINTA("#LPMSTATUS# 0 -> [%lu] 1 -> [%lu] 2 -> [%lu] \n",
	lpm_stats[0],lpm_stats[1],lpm_stats[2]);
#else
  printf("LPM_CONF_STATS is disabled \n");
#endif
}

void
reset_stats(void)
{
#if RIMESTATS_CONF_ENABLED
	RIMESTATS_GET(tx)=0;RIMESTATS_GET(rx)=0;
	RIMESTATS_GET(reliabletx)=0;
	RIMESTATS_GET(reliablerx)=0;
	RIMESTATS_GET(rexmit)=0; RIMESTATS_GET(acktx)=0; RIMESTATS_GET(noacktx)=0;
	RIMESTATS_GET(ackrx)=0; RIMESTATS_GET(timedout)=0; RIMESTATS_GET(badackrx)=0;
	RIMESTATS_GET(toolong)=0; RIMESTATS_GET(tooshort)=0;
	RIMESTATS_GET(badsynch)=0; RIMESTATS_GET(badcrc)=0;
	RIMESTATS_GET(contentiondrop)=0; RIMESTATS_GET(sendingdrop)=0;
	RIMESTATS_GET(lltx)=0; RIMESTATS_GET(llrx)=0;
#endif

#if ENERGEST_CONF_ON
	energest_total_time[ENERGEST_TYPE_CPU].current=0;
	energest_total_time[ENERGEST_TYPE_LPM].current=0;
	energest_total_time[ENERGEST_TYPE_IRQ].current=0;
	energest_total_time[ENERGEST_TYPE_LED_GREEN].current=0;
	energest_total_time[ENERGEST_TYPE_LED_YELLOW].current=0;
	energest_total_time[ENERGEST_TYPE_LED_RED].current=0;
	energest_total_time[ENERGEST_TYPE_TRANSMIT].current=0;
	energest_total_time[ENERGEST_TYPE_LISTEN].current=0;
	energest_total_time[ENERGEST_TYPE_SENSORS].current=0;
	energest_total_time[ENERGEST_TYPE_SERIAL].current=0;
#endif

#if LPM_CONF_STATS
	lpm_stats[0]=0;lpm_stats[1]=0;lpm_stats[2]=0;
	printf("statistiche inizializzate \n");
#endif
}
/*---------------------------------------------------------------------------*/
