/*
 *	I2CMultiUser.java
 * 
 *	Copyright (c) 2013 DiSBeF, University of Urbino.
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
 * Simple I2C test application to read data from embedded eeprom.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.digitalio.bus.I2C;
import javax.virtualsense.actuators.Leds;
import java.lang.Runtime;
import javax.virtualsense.sensors.*;


public class I2CReader
{
	private static byte SI7020_ADDRESS = (byte)0x40;
	private static byte SI7020_READ_TEMP_NOHOLD = (byte)0xF3;
	private static byte SI7020_READ_HUMID_NOHOLD = (byte)0xF5;
	private static byte SI7020_RESET = (byte)0xFE;
	
	
    public static void motemain()
    {
    	while(true) {
	    	System.out.print("Temperature: ");
	    	System.out.print(Temperature.getValue());
	    	System.out.println("mC");
	    	Thread.sleep(500);
	    	
	    	System.out.print("Pressure: ");
	    	System.out.print(Pressure.getValue());
	    	System.out.println("hPa");
	    	Thread.sleep(500);
	    	
	    	
	    	
	    	
	    	System.out.print("Temp SI7020: ");
	    	System.out.print(readTempSI7020());
	    	System.out.println("");
	    	Thread.sleep(500);
	    	
	    	
	    	
	    	/* byte ret = 0;
	    	 byte eepromAddress = (byte)0xA6;
	    	 short address = 3;
	 
	    	 I2C.enable();
	    	 I2C.start();
	
	    	 I2C.write((eepromAddress));
	    	 I2C.write(((byte)(address >> 8)));
	    	 I2C.write(((byte)(address & 0xff)));
	    	 I2C.stop();
	
	    	 I2C.start();
	    	 I2C.write((byte)((eepromAddress) | 1));
	    	 ret = I2C.read(false);
	
	    	 I2C.stop();
	    	 I2C.disable();
	    	 System.out.print("Read id from eeprom ");
	    	 System.out.println(ret);
	        */
    	}
       
    }
    
    private static short readTempSI7020() {
    	
    	short temp = (short)0;
    	
    	 I2C.enable();
    	 
    	 I2C.start();
    	 I2C.write(SI7020_ADDRESS);
    	 I2C.write(SI7020_READ_TEMP_NOHOLD);
    	 I2C.stop();

    	 boolean isIdle = true;
    	 
    	 do {
    		 Thread.sleep(200);
    		 I2C.start();
    		 isIdle = I2C.write((byte)(SI7020_ADDRESS | 1));
    		 if(isIdle)
    	    	 I2C.stop();
    	 }
    	 while(isIdle);

    	 System.out.print("Read bytes: ");
    	 System.out.print(I2C.read(false));
    	 System.out.print(" ");
    	 System.out.println(I2C.read(false));
    	 
    	 
    	 I2C.stop();
    	 I2C.disable();
    		
    	 return temp;
    }
    
    
    
	/*uint16_t temp = 0;

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
	uint16_t read = ((uint16_t)I2CMasterDataGet()) << 8;

	I2CMasterControl(I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy());
	if(I2CMasterErr() == I2C_MASTER_ERR_NONE){
		read += (uint16_t)I2CMasterDataGet();
	}else{
		PRINTF("SI7020: error on read temp (read last byte)\n");
	}

	// Conversion returned value in celsius degrees
	temp = (uint16_t)((((uint32_t)read * 21965)/ 81920)-4685);	// °C = ((175,72*16bit_value)/65536)-46,85

	// Power down sensor
	SI7020_POWER_DOWN();

	return temp;*/
    
}
