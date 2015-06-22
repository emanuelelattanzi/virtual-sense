/*
 *	javax_virtualsense_digitalio_InterruptPin.c
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
 * DigitalIO InterruptPin native methods.
 *
 * @author Emanuele Lattanzi
 *
 */


#include "common/execution/execution.h"
#include "common/array.h"
#include "common/darjeeling.h"
#include "node-id.h"

#include "contiki.h"
#include "power-interface.h"

#include "dev/digitalio.h"
#include "board.h"


#include "base_definitions.h"


dj_thread * receiver_thread;
void javax_virtualsense_digitalio_InterruptPin_void__createInterruptPin_boolean_short_short(){
	uint16_t push_pull = dj_exec_stackPopShort();
	uint16_t port = dj_exec_stackPopShort();
	uint16_t falling = dj_exec_stackPopShort();

	uint32_t int_pin = 0;
	switch(port){
		case INT0:
			int_pin = INT0_PIN;
			break;
		case INT1:
			int_pin = INT1_PIN;
			break;
		case INT2:
			int_pin = INT2_PIN;
			break;
		/*case INT3:
			int_pin = INT3_PIN;
			break;*/
		default:
			break;
	}

	uint32_t pin_drive = 0;
	switch(push_pull){
		case 0:
			pin_drive = IOC_OVERRIDE_PDE;
			break;
		case 1:
			pin_drive = IOC_OVERRIDE_PUE;
			break;
		case 2:
			pin_drive = IOC_OVERRIDE_DIS;
			break;
		default:
			break;
	}

	init_interrupt((uint8_t)falling, int_pin, pin_drive);
}

void javax_virtualsense_digitalio_InterruptPin_void__waitForInterrupt_short(){
	uint16_t port = dj_exec_stackPopShort();
	dj_thread *thread = dj_exec_getCurrentThread();
	thread->sem_id = (port+1); //Increase the port number by 1 to match semaphore number (zero is not allowed)
	thread->status = THREADSTATUS_BLOCKED_FOR_IO;
	dj_exec_breakExecution();
}

