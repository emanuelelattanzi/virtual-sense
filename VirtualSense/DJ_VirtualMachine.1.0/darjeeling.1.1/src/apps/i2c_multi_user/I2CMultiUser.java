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

public class I2CMultiUser
{
    public static void motemain() {
    	
	 	byte b = (byte)0x02;
    	byte adr = (byte)0x57;
    	byte write[] = new byte[130];
    	byte baseAdr[] = new byte[2];
	 	baseAdr[0] = (byte)0x00;
	 	baseAdr[1] = (byte)0x00;
    	
    	// Wait for eeprom
    	Thread.sleep(1500);
	 
    	// Insert base address before byte to write
    	write[0] = baseAdr[0];
 		write[1] = baseAdr[1];
    	for(int i = 2; i < 130; i++){
    		write[i] = b++;
    	}
    	 
    	I2C.enable();
    	 
    	// Write page to eeprom and wait for idle
    	System.out.print("write to eeprom: ");
    	System.out.println(I2C.write(adr, write)?"ACK":"NOACK"); 
    	Thread.sleep(1500); 
    	
    	// Write base address and read page
    	System.out.println("read from eeprom:");
    	I2C.write(adr, baseAdr);
    	byte read[] = I2C.read(adr, (short)128);
    	 
    	for(int j = 0; j < read.length; j++){
    		System.out.print("read[");System.out.print(j);System.out.print("]: ");System.out.print(read[j]);System.out.print(" test: ");System.out.println((write[j+2]==read[j])?"OK":"FAIL");
    	}
    }
}
