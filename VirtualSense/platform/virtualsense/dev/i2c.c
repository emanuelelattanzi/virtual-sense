/*
 *	i2c.c
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
 * I2C access definitions.
 *
 * @author Matteo Dromedari
 *
 */


#include <contiki.h>
//#include <dev/spi.h>
//#include <dev/leds.h>

#include "board.h"
#include "i2c.h"


/*
 * set EEPROM pin to height impedence in order to
 * reduce current drain. Then i2c_enable will remember these
 * configuration an will restore them after use
 */
void i2c_init(){
    //
    // Set IO clock to the same as system clock
    //
    SysCtrlIOClockSet(SysCtrlClockGet());

	//
	//  The I2C peripheral must be enabled before use.
	//
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_I2C);

	//
	// Do reset of I2C module
	//
	SysCtrlPeripheralReset(SYS_CTRL_PERIPH_I2C);

	//
	// Configure I2C pins
	//
	GPIOPinTypeI2C(I2C_SCL_PORT, I2C_SCL_PIN);
	GPIOPinTypeI2C(I2C_SDA_PORT, I2C_SDA_PIN);

    //
    // Configure pins as peripheral input and output
    //
    IOCPinConfigPeriphInput(I2C_SCL_PORT, I2C_SCL_PIN,
                            IOC_I2CMSSCL);
    IOCPinConfigPeriphInput(I2C_SDA_PORT, I2C_SDA_PIN,
                            IOC_I2CMSSDA);
    IOCPinConfigPeriphOutput(I2C_SCL_PORT, I2C_SCL_PIN,
                             IOC_MUX_OUT_SEL_I2C_CMSSCL);
    IOCPinConfigPeriphOutput(I2C_SDA_PORT, I2C_SDA_PIN,
                             IOC_MUX_OUT_SEL_I2C_CMSSDA);

    //
    // Enable and initialize the I2C master module.  Use the system clock for
    // the I2C module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.  For this example we will use a data rate of 100kbps.
    //
    I2CMasterInitExpClk(SysCtrlClockGet(), false);
}



/*
 * Grab SDA and SCL pins for exclusive use but remember old
 * configuration so that it may be restored when we are done.
 */
void
i2c_enable(void)
{
#if 0 //TODO to implement
  unsigned char sda_scl = BV(SDA)|BV(SCL);


  old_pxsel = I2C_PxSEL & sda_scl;
  old_pxout = I2C_PxOUT & sda_scl;
  old_pxdir = I2C_PxDIR & sda_scl;

  spi_busy = 1;

  I2C_PxSEL &= ~sda_scl;

  I2C_PxOUT &= ~sda_scl;

  I2C_PxDIR |=  BV(SCL);		/* SCL Output */
  I2C_PxDIR &= ~BV(SDA);		/* SDA Input */
#endif
}

/*
 * Restore bus to what it was before i2c_enable.
 *
 */
void
i2c_disable(void)
{
#if 0 //TODO to implement
  unsigned char not_sda_scl = ~(BV(SDA)|BV(SCL));

  I2C_PxDIR = (I2C_PxDIR & not_sda_scl) | old_pxdir;
  I2C_PxOUT = (I2C_PxOUT & not_sda_scl) | old_pxout;
  I2C_PxSEL = (I2C_PxSEL & not_sda_scl) | old_pxsel;

  spi_busy = 0;
#endif
}



/*
 * Return true if communication is done.
 */
uint8_t i2c_write(uint8_t address, uint8_t * values, uint16_t len){
	uint8_t ret = 0x01;
	uint32_t cmd = 0;

	//printf("i2cwrite address: %x\n", address);

	// Set I2Cmodule in write mode to specified address
	I2CMasterSlaveAddrSet(address, false);

	uint16_t i = 0;
	for(i = 0; i < len; i++) {

		if(len == 1)
			cmd = I2C_MASTER_CMD_SINGLE_SEND;
		else
			if(i == 0)
				cmd = I2C_MASTER_CMD_BURST_SEND_START;
			else if(i > 0 && i < len -1)
				cmd = I2C_MASTER_CMD_BURST_SEND_CONT;
			else if(i == len -1)
				cmd = I2C_MASTER_CMD_BURST_SEND_FINISH;

		// Write i byte on I2C bus
		I2CMasterDataPut(values[i]);
		//printf("w values[%d] = %x\n", i, values[i]);
		I2CMasterControl(cmd);
		while(I2CMasterBusy());
		if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
			ret &= 0x00;
		}
	}

	return ret;
}



uint16_t i2c_read(uint8_t address, uint8_t * values, uint16_t len){
	uint16_t readBytes = 0;
	uint32_t cmd = 0;

	//printf("i2cread address: %x\n", address);

	// Set I2Cmodule in read mode to specified address
	I2CMasterSlaveAddrSet(address, true);

	uint16_t i = 0;
	for(i = 0; i < len; i++){

		if(len == 1)
			cmd = I2C_MASTER_CMD_SINGLE_RECEIVE;
		else
			if(i == 0)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_START;
			else if(i > 0 && i < len -1)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_CONT;
			else if(i == len -1)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_FINISH;

		// Read i byte from I2C bus
		I2CMasterControl(cmd);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			values[i] = I2CMasterDataGet();
			readBytes++;
			//printf("r values[%d] = %x\n", i, values[i]);
		}
	}

	return readBytes;
}



/*
 * Return true if communication is done.
 */
uint8_t i2c_write_burst(uint8_t dev_adr, uint8_t reg_adr, uint8_t * values, uint16_t len){
	uint8_t ret = 0x01;
	uint32_t cmd = 0;

	//printf("i2cwrite address: %x\n", address);

	// Set I2Cmodule in write mode to specified address
	I2CMasterSlaveAddrSet(dev_adr, false);

	// Write register address on I2C bus
	I2CMasterDataPut(reg_adr);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		ret &= 0x00;
	}

	uint16_t i = 0;
	for(i = 0; i < len; i++) {

		if(i == len-1)
			cmd = I2C_MASTER_CMD_BURST_SEND_FINISH;
		else
			cmd = I2C_MASTER_CMD_BURST_SEND_CONT;

		// Write i byte on I2C bus
		I2CMasterDataPut(values[i]);
		//printf("w values[%d] = %x\n", i, values[i]);
		I2CMasterControl(cmd);
		while(I2CMasterBusy());
		if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
			ret &= 0x00;
		}
	}

	return ret;
}



uint16_t i2c_read_burst(uint8_t dev_adr, uint8_t reg_adr, uint8_t * values, uint16_t len){
	uint16_t readBytes = 0;
	uint32_t cmd = 0;

	//printf("i2cread address: %x\n", address);

	// Set I2Cmodule in write mode to specified address
	I2CMasterSlaveAddrSet(dev_adr, false);

	// Write register address on I2C bus
	I2CMasterDataPut(reg_adr);
	I2CMasterControl(I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy());
	if(I2CMasterErr() != I2C_MASTER_ERR_NONE){
		readBytes = 0x00;
	}

	// Set I2Cmodule in read mode to specified address
	I2CMasterSlaveAddrSet(dev_adr, true);

	uint16_t i = 0;
	for(i = 0; i < len; i++){

		if(len == 1)
			cmd = I2C_MASTER_CMD_SINGLE_RECEIVE;
		else
			if(i == 0)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_START;
			else if(i > 0 && i < len -1)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_CONT;
			else if(i == len -1)
				cmd = I2C_MASTER_CMD_BURST_RECEIVE_FINISH;

		// Read i byte from I2C bus
		I2CMasterControl(cmd);
		while(I2CMasterBusy());
		if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
			values[i] = I2CMasterDataGet();
			readBytes++;
			//printf("r values[%d] = %x\n", i, values[i]);
		}
	}

	return readBytes;
}

