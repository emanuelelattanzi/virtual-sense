/*
 *  SI7020.c
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
 * The SI7020 temperature humidity sensor driver.
 *
 * @author Matteo Dromedari
 *
 */


#include "board.h"

#include "i2c.h"
#include "SI7020.h"


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif

// MET !!! Cannot shutdown SI7020 because invalidates the SDA SCL lines !!!
void SI7020_POWER_UP(){
	//GPIOPinWrite(SI7020_POWER_PORT, SI7020_POWER_PIN, SI7020_POWER_PIN);
}

void SI7020_POWER_DOWN(){
	//GPIOPinWrite(SI7020_POWER_PORT, SI7020_POWER_PIN, SI7020_POWER_PIN & 0x00);
}

void init_SI7020(){

	// Configure eeprom power pin and shut down it
	//GPIOPinTypeGPIOOutput(SI7020_POWER_PORT, SI7020_POWER_PIN);
	//SI7020_POWER_DOWN();

	// Init i2c bus
	i2c_init();
}

uint8_t isIdle_SI7020(){

	uint32_t isIdle = 0x00;

	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	isIdle = I2CMasterErr();
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
	while(I2CMasterBusy());

	return (uint8_t)isIdle;
}

void waitForIdle_SI7020(){

	PRINTF("SI7020: wait for sensor...");
	while(isIdle_SI7020()){
		PRINTF(".");
	}
	PRINTF("sensor up!\n");
}

uint16_t read_temp_SI7020(void){
	uint16_t temp = 0;

	// Power sensor and wait is up
	SI7020_POWER_UP();
	waitForIdle_SI7020();

	// Set I2Cmodule in write mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, false);

	// Write command for start temperature conversion
	I2CMasterDataPut(SI7020_READ_TEMP_NOHOLD);
	I2CMasterControl(I2C_MASTER_CMD_SINGLE_SEND);
	clock_delay(50000);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("SI7020: error on write command (temp read)\n");
	}

	// Set I2Cmodule in read mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);

	// Wait for temperature conversion
	do{
		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy());
	}
	while(I2CMasterErr() != I2C_MASTER_ERR_NONE);

	// Read converted temperature value from SI7020
	uint32_t read = (I2CMasterDataGet() << 8) & 0x0000FF00;
	PRINTF("SI7020: (b1 << 8) & 0x0000FF00: %d ", read);

	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		read |= I2CMasterDataGet();
		PRINTF("SI7020: ((b1<<8)&0x0000FF00) | b2: %d\n", read);
	}else{
		PRINTF("SI7020: error on read temp (read last byte)\n");
	}

	// Conversion returned value in celsius degrees
	temp = (uint16_t)(((read * 21965)/ 81920)-4685);	// °C = ((175,72*16bit_value)/65536)-46,85

	// Power down sensor
	SI7020_POWER_DOWN();

	return temp;
}



uint16_t read_humid_SI7020(void){
	uint16_t humid = 0;

	// Power sensor and wait is up
	SI7020_POWER_UP();
	waitForIdle_SI7020();

	// Set I2Cmodule in write mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, false);

	// Write command for start humidity conversion
	I2CMasterDataPut(SI7020_READ_HUMID_NOHOLD);
	I2CMasterControl(I2C_MASTER_CMD_SINGLE_SEND);
	clock_delay(50000);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		PRINTF("SI7020: error on write command (humid read)\n");
	}

	// Set I2Cmodule in read mode to SI7020 address
	I2CMasterSlaveAddrSet(SI7020_ADDRESS, true);

	// Wait for humidity conversion
	do{
		I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy());
	}
	while(I2CMasterErr() != I2C_MASTER_ERR_NONE);

	// Read converted humidity value from SI7020
	uint32_t read = (I2CMasterDataGet() << 8) & 0x0000FF00;
	PRINTF("SI7020: (b1 << 8) & 0x0000FF00: %d ", read);

	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		read |= (I2CMasterDataGet() & 0x000000FC);
		PRINTF("SI7020: ((b1<<8)&0x0000FF00) | (b2 &0x000000FC): %d\n", read);
	}else{
		PRINTF("SI7020: error on read hum (read last byte)\n");
	}

	// Conversion returned value in %RH
	humid = (uint16_t)((((uint32_t)read * 125)/ 65536)-6);		// %RH = ((16bit_value*125)/65536)-6

	// Power down sensor
	SI7020_POWER_DOWN();

	return humid;
}
