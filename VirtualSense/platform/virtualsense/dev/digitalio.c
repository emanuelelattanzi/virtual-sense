/*
 *	digitalio.c
 *
 *  Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * Digital I/O access definitions.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "dev/digitalio.h"

#include "cpu.h"
#include "dev/pcf2123_spi.h"
#include "contiki.h"
#include "board.h"

#include "lib/hw_memmap.h"
#include "lib/hw_ioc.h"
#include "lib/hw_ints.h"
#include "lib/_interrupt.h"
#include "lib/_gpio.h"
#include "lib/_ioc.h"
#include "lib/_sys_ctrl.h"

extern void GPIOIntHandler(void);

PROCESS(digitalio_driver_process, "DIGITALIO_process");
static uint8_t flag = 10;
static void (*registered_call_back)(uint8_t port);

void init_digitalio_interface(void ( *callback)(uint8_t port)){
	registered_call_back = callback;
	process_start(&digitalio_driver_process, NULL);
}

/*---------------------------------------------------------------------------*/
void init_interrupt(uint8_t on_falling, uint32_t int_pin, uint32_t pin_drive){
	//
	// Register interrupt handler function
	//
	IntRegister(INT_GPIO, GPIOIntHandler);
	//
	// Set IO clock to the same as system clock
	//
	SysCtrlIOClockSet(SysCtrlClockGet());
    //
    // Set "INT_PORT" Port "int_pin" Pin as an input.  This configures the pin as an input using
    // "pin_drive" push-pull mode.
    //
    GPIOPinTypeGPIOInput(INT_PORT, int_pin);
    IOCPadConfigSet(INT_PORT, int_pin, pin_drive);
    //
	// Set the type of interrupt for Port "INT_PORT" - Pin "int_pin":
	// rising edge, falling edge, both edges, low level, or high level.
	//
	GPIOIntTypeSet(INT_PORT, int_pin, on_falling<1?GPIO_RISING_EDGE:GPIO_FALLING_EDGE);
	//
	// Enable the GPIO interrupt.
	//
	GPIOPinIntEnable(INT_PORT, int_pin);
    //
    // Enable processor interrupts.
    //
    IntMasterEnable();
    //
    // Enable the GPIOC interrupt on the processor (NVIC).
    //
    IntEnable(INT_GPIO);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(digitalio_driver_process, ev, data)
{
  int len;
  PROCESS_BEGIN();

  //printf("DIGITALIO_process: started\n");

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    //printf("DIGITALIO_process: calling callback\n");
    registered_call_back(flag);
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
void
GPIOIntHandler(void)
{
	printf("Interrupt\n");
    //
    // Get the masked interrupt status.
    //
    uint32_t ui32GPIOIntStatus = GPIOPinIntStatus(INT_PORT, true);

    //
    // Handle received interrupt
    //
	if(ui32GPIOIntStatus & INT0_PIN){
		flag = INT0;
		process_poll(&digitalio_driver_process);
	}else if(ui32GPIOIntStatus & INT1_PIN){
		flag = INT1;
		process_poll(&digitalio_driver_process);
	}else if(ui32GPIOIntStatus & INT2_PIN){
		flag = INT2;
		process_poll(&digitalio_driver_process);
	}/*else if(ui32GPIOIntStatus & INT3_PIN){
		flag = INT3;
		process_poll(&digitalio_driver_process);
	}*/else if(ui32GPIOIntStatus & RTC_INT_PIN){
		flag = INT_RTC;
#ifdef PLATFORM_HAS_RTC_PCF2123
		printf("RTC interr\n");
		RTC_clear_interrupt();
		RTC_disable_all_interrupts();
#endif
		//LPM4_EXIT;
	}

    //
    // Acknowledge the GPIO  - Pin n interrupt by clearing the interrupt flag.
    //
    GPIOPinIntClear(INT_PORT, ui32GPIOIntStatus);
}


/*---------------------------------------------------------------------------*/
void init_dio(uint8_t direction, uint16_t port){
	switch(port){
		case DIO0:
			direction>0?(GPIOPinTypeGPIOInput(DIO_PORT, DIO0_PIN)):(GPIOPinTypeGPIOOutput(DIO_PORT, DIO0_PIN));
			break;
		case DIO1:
			direction>0?(GPIOPinTypeGPIOInput(DIO_PORT, DIO1_PIN)):(GPIOPinTypeGPIOOutput(DIO_PORT, DIO1_PIN));
			break;
		case DIO2:
			direction>0?(GPIOPinTypeGPIOInput(DIO_PORT, DIO2_PIN)):(GPIOPinTypeGPIOOutput(DIO_PORT, DIO2_PIN));
			break;
		case DIO3:
			direction>0?(GPIOPinTypeGPIOInput(DIO_PORT, DIO3_PIN)):(GPIOPinTypeGPIOOutput(DIO_PORT, DIO3_PIN));
			break;
		default:
			break;
	}
}

/*---------------------------------------------------------------------------*/
uint8_t read_pin(uint16_t port){
	uint8_t ret = 0;

	switch(port){
		case DIO0:
			ret = ((uint8_t)GPIOPinRead(DIO_PORT, DIO0_PIN) & DIO0_PIN);
			break;
		case DIO1:
			ret = ((uint8_t)GPIOPinRead(DIO_PORT, DIO1_PIN) & DIO1_PIN);
			break;
		case DIO2:
			ret = ((uint8_t)GPIOPinRead(DIO_PORT, DIO2_PIN) & DIO2_PIN);
			break;
		case DIO3:
			ret = ((uint8_t)GPIOPinRead(DIO_PORT, DIO3_PIN) & DIO3_PIN);
			break;
		default:
			break;
	}
	return ret;
}

/*---------------------------------------------------------------------------*/
void write_pin(uint16_t port, uint8_t value){
	switch(port){
		case DIO0:
			GPIOPinWrite(DIO_PORT, DIO0_PIN, value>0?(DIO0_PIN):(0x00));
			break;
		case DIO1:
			GPIOPinWrite(DIO_PORT, DIO1_PIN, value>0?(DIO1_PIN):(0x00));
			break;
		case DIO2:
			GPIOPinWrite(DIO_PORT, DIO2_PIN, value>0?(DIO2_PIN):(0x00));
			break;
		case DIO3:
			GPIOPinWrite(DIO_PORT, DIO3_PIN, value>0?(DIO3_PIN):(0x00));
			break;
		default:
			break;
		}
}
