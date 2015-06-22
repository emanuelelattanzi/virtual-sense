/*
 *	javax_virtualsense_sensosors_Temperature.c
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
 * Temperature access methods.
 *
 * @author Matteo Dromedari
 *
 */


// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"
#include "dev/SI7020.h"
#include "board.h"

#include "lib/_sys_ctrl.h"
#include "lib/hw_cctest.h"
#include "lib/hw_rfcore_xreg.h"


#define CONST 0.58134 //(VREF / 2047) = (1190 / 2047), VREF from Datasheet
#define OFFSET_DATASHEET_25C 827 // 1422*CONST, from Datasheet
#define TEMP_COEFF (CONST * 4.2) // From Datasheet
#define OFFSET_0C (OFFSET_DATASHEET_25C - (25 * TEMP_COEFF))


//public static native int getValue();
void javax_virtualsense_sensors_Temperature_short_getValue()
{
	dj_exec_stackPushShort(read_temp_SI7020());
}


//public static native short getBoardValue();
void javax_virtualsense_sensors_Temperature_short_getBoardValue()
{
	//
	// Enable RF Core (needed to enable temp sensor)
	//
	//SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_RFC);
	//
	// Connect temp sensor to ADC
	//
	//HWREG(CCTEST_TR0) |= CCTEST_TR0_ADCTM;
	//
	// Enable the temperature sensor
	//
	//HWREG(RFCORE_XREG_ATEST) = 0x01;

	uint16_t read = adc_read(SOCADC_TEMP_SENS, ADC_INTREF);

	double temp = (((read * CONST) - OFFSET_0C) / TEMP_COEFF);

	dj_exec_stackPushShort((uint16_t)(temp * 1000));
}



