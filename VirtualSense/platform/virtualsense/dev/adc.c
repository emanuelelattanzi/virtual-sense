/*
 *	adc.c
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
 * ADC access definitions.
 *
 * @author Emanuele Lattanzi
 *
 */

//#include "dev/adc.h"
#include "contiki.h"
#include "board.h"
#include "eeprom.h"

#include "adc.h"
#include "lib/_ioc.h"
#include "lib/_sys_ctrl.h"




/*---------------------------------------------------------------------------*/
void adc_init(void){
	//
	// Set IO clock to the same as system clock
	//
	SysCtrlIOClockSet(SysCtrlClockGet());
	//
	// Configure all ADC port as analog input without pull-up
	//
	GPIODirModeSet(ADC_PORT, ADC0_PIN, GPIO_DIR_MODE_HW);
	IOCPadConfigSet(ADC_PORT, ADC0_PIN, IOC_OVERRIDE_ANA);
	GPIODirModeSet(ADC_PORT, ADC1_PIN, GPIO_DIR_MODE_HW);
	IOCPadConfigSet(ADC_PORT, ADC1_PIN, IOC_OVERRIDE_ANA);
	GPIODirModeSet(ADC_PORT, ADC2_PIN, GPIO_DIR_MODE_HW);
	IOCPadConfigSet(ADC_PORT, ADC2_PIN, IOC_OVERRIDE_ANA);
	//GPIODirModeSet(ADC_PORT, ADC3_PIN, GPIO_DIR_MODE_HW);
	//IOCPadConfigSet(ADC_PORT, ADC3_PIN, IOC_OVERRIDE_ANA);
}
/*---------------------------------------------------------------------------*/


uint16_t adc_read(uint32_t channel, uint32_t ref){

	int16_t sample = 0;
  	//
	// Configure ADC, Internal reference, 512 decimation rate (12bit)
	//
  	SOCADCSingleConfigure(SOCADC_12_BIT, ref);
	//
	// Trigger single conversion on specified channel
	//
	SOCADCSingleStart(channel);
	//
	// Wait until conversion is completed
	//
	while(!SOCADCEndOfCOnversionGet())
	{
	}
	//
	// Get data and shift down based on decimation rate
	//
	sample = (int16_t)(SOCADCDataGet() >> SOCADC_12_BIT_RSHIFT);

	return (uint16_t)((sample > 0x7FF)?0:sample);
}
/*---------------------------------------------------------------------------*/







