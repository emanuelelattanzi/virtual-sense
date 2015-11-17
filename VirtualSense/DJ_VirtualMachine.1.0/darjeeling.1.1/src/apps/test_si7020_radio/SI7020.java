/*
 *	SI7020.java
 * 
 *	Copyright (c) 2011 DiSBeF, University of Urbino.
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
 * Driver for Silab SI7020 Temperatue-humidity sensors.
 * 
 * @author Matteo Dromedari
 *
 */


import javax.virtualsense.digitalio.bus.I2C;



public class SI7020
{	
	public static byte SENSOR_ADDRESS = (byte)0x80;
	public static byte CMD_READ_TEMP_NOHOLD = (byte)0xF3;
	public static byte CMD_READ_HUMID_NOHOLD = (byte)0xF5;
	public static byte CMD_READ_USER_REG_1 = (byte)0xE7;
	public static byte CMD_WRITE_USER_REG_1 = (byte)0xE6;
	public static byte CMD_RESET = (byte)0xFE;
	
	
	public SI7020(){
	}
    
	public short readTemp() {
    	
    	short temp = (short)0;
    	 
    	 I2C.start();
    	 I2C.write(SENSOR_ADDRESS);
    	 I2C.write(CMD_READ_TEMP_NOHOLD);
    	 
    	 do {
    		 System.out.print("");
    		 I2C.start();
    	 }
    	 while(!I2C.write((byte)(SENSOR_ADDRESS | 1)));

    	 byte b1 = I2C.read(true);
    	 System.out.print("");
    	 byte b2 = I2C.read(false);
    	 
    	 I2C.stop();
    	 
    	 int read = ((int)b1 << 8) & 0x0000FF00;
    	 read |= (int)b2;
    	 
    	 temp = (short)(((read * 21965)/ 81920)-4685);
    		
    	 return temp;
    }
	
	public short readHum() {
    	
    	short hum = (short)0;
    	 
    	 I2C.start();
    	 I2C.write(SENSOR_ADDRESS);
    	 I2C.write(CMD_READ_HUMID_NOHOLD);
    	 
    	 do {
    		 System.out.print("");
    		 I2C.start();
    	 }
    	 while(!I2C.write((byte)(SENSOR_ADDRESS | 1)));

    	 byte b1 = I2C.read(true);
    	 System.out.print("");
    	 byte b2 = I2C.read(false);
     	 
    	 I2C.stop();

    	 int read = ((int)b1 << 8) & 0x0000FF00;
    	 read |= ((int)b2 & 0x0000FC);
    	 
    	 hum = (short)(((read * 12500)/ 65536)-600);
    	 
    	 if(hum > (short)10000)
    		 hum = (short)10000;
    	 
    	 if(hum < (short)0)
    		 hum = (short)0;
    		
    	 return hum;
    }
    
}
    	
    	
    	
