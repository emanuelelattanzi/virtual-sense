/*
 *  spi_UCB1.h
 *
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
 *
 *	This file is part of VirtualSense.
 *
 *	VirtualSense is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	VirtualSense is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * 	UCB1 spi management
 *
 * @author Emanuele Lattanzi
 *
 */

#include "contiki.h"
#include "spi_UCB1.h"


static uint8_t SPI_up = 0;
/* initialize the spi interface */
void spi_UCB1_init(uint8_t clock_divider){
	if(!SPI_up){

		/* Make sure SIMO is configured as output */
		P3DIR |= BIT7;
		/* Make sure MISO is configured as input */
		P5DIR &= ~BIT4;
		P5DIR |= BIT5;
		//P2DIR |= BIT1;

		P5SEL |= BIT5 + BIT4; 				/// P5.5 (UCB1CLK) e P5.4 (UCB1MISO)
		P3SEL |= BIT7;						///  P3.7 (UCB1MOSI)

	/* Put state machine in reset */
		UCB1CTL1 |= UCSWRST;

		/* 3-pin, 8-bit SPI master, rising edge capture */
		UCB1CTL0 |= UCCKPH /*UCCKPL*/ | UCSYNC | UCMSB | UCMST;
		/* Select SMCLK */
		UCB1CTL1 = UCSSEL_2;

		/* max 8MHz spi for the RF module*/
		UCB1BR0 = clock_divider;
		UCB1BR1 = 0;

		/* Initialize USCI state machine */
		UCB1CTL1 &= ~UCSWRST;
		SPI_up = 1;
		//---printf("SPI initialized\n");

	}
}
uint8_t spi_UCB1_is_up(void){
	return SPI_up;
}

void spi_UCB1_shutdown(void){
	/* Put state machine in reset */
	UCB1CTL1 |= UCSWRST;
	/* Select SMCLK */
	UCB1CTL1 = 0x0;

	P5SEL &= ~(BIT5 + BIT4); 				/// P5.5 (UCB1CLK) e P5.4 (UCB1MISO)
	P3SEL &= ~BIT7;						///  P3.7 (UCB1MOSI)

	P5DIR &= ~(BIT5 + BIT4); 				/// P5.5 (UCB1CLK) e P5.4 (UCB1MISO)
	P3DIR &= ~BIT7;						///  P3.7 (UCB1MOSI)

	SPI_up = 0;

}

u8_t UCB1_SPI_TXRX(u8_t x)
{
    UCB1_SPI_RX_NOT_READY();
    UCB1_SPI_TX_REG = x;
    while (!UCB1_SPI_RX_IS_READY());
    return UCB1_SPI_RX();
}
