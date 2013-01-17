/*
 *  uart.c
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
 * UART code for MSP43054xx.
 *
 * @author Emanuele Lattanzi
 *
 */
#ifndef __UART_H__
#define __UART_H__

#include "msp430.h"

void uart_set_input(int (*input)(unsigned char c));

//void uartSend(char string[]);
void uartSendChar(char c);
void putChar(char c);
void uartInit(unsigned char clock_speed);
void uartShutDown(void);
//char uartGetChar(void);

//TODO: verify
//void usbTest( void );
#endif /* __UART1_H__ */
