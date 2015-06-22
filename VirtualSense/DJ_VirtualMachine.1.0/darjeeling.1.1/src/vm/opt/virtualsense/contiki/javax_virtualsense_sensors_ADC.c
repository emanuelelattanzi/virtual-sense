/*
 *	javax_virtualsense_sensosors_ADC.c
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
 * ADC access methods.
 *
 * @author Emanuele Lattanzi
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// generated at infusion time
#include "base_definitions.h"

#include "common/execution/execution.h"
#include "common/heap/heap.h"
#include "dev/adc.h"
#include "dev/board.h"



// short javax.virtualsense.sensors.ADC._read(short channel short ref short vdd);
void javax_virtualsense_sensors_ADC_short__read_short_short_short()
{
	uint16_t read = 0;
	uint16_t volt_ref = dj_exec_stackPopShort();	// Volt reference to use for conversion

	uint32_t SOCADC_ref = 0;
	uint16_t ref = dj_exec_stackPopShort();
	switch(ref){
		case ADC_INTREF:{
			SOCADC_ref = SOCADC_REF_INTERNAL;
			volt_ref = ADC_INT_REF;					// Set volt_ref to internal volt reference
			}
			break;
		case ADC_VDDREF:
			SOCADC_ref = SOCADC_REF_AVDD5;
			break;
		default:
			break;
	}

	uint16_t ch = dj_exec_stackPopShort();
	switch(ch){
		case ADC0:
			read = (uint16_t)((uint32_t)(adc_read(ADC0_CH, SOCADC_ref) * volt_ref) / 2047);
			break;
		case ADC1:
			read = (uint16_t)((uint32_t)(adc_read(ADC1_CH, SOCADC_ref) * volt_ref) / 2047);
			break;
		case ADC2:
			read = (uint16_t)((uint32_t)(adc_read(ADC2_CH, SOCADC_ref) * volt_ref) / 2047);
			break;
		/*case ADC3:
			read = (uint16_t)((uint32_t)(adc_read(ADC3_CH, SOCADC_ref) * volt_ref) / 2047);
			break;*/
		case ADCVDD:
			read = ((uint16_t)((uint32_t)(adc_read(ADCVDD_CH, SOCADC_ref) * volt_ref) / 2047)) * 3;		// *3 because ADCVDD channel returns Vdd/3
			break;
		default:
			break;
	}

	dj_exec_stackPushShort(read);
}





