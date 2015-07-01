/*
 *	Accelerometer.java
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
 package javax.virtualsense.actuators.evm.display;


import javax.virtualsense.digitalio.bus.*;

/**
 * Driver for MIDAS i2c display of evm.
 * 
 * @author Matteo Dromedari
 *
 */
public class Display
{	
	private static Display instance;
	
	// DISPLAY I2C address
	private static final String HELLO_WORLD = "    VS32 Evm    " + " virtualsense.it";
	private static final byte DISPLAY_ADR = (byte)0x3E;
	private static final byte DISPLAY_CONF_REG = (byte)0x00;
	private static final byte DISPLAY_WRITE_REG = (byte)0x40;
	private static final byte DISPLAY_CLEAR_CMD = (byte)0x01;
	private static final byte DISPLAY_CR_CMD = (byte)0xC0;
	
	
	/**
	 * Return an instance of Display class for manage evm dispaly.	 
	 * 
	 */
	public static Display getInstance() {
		if(instance == null)
			instance = new Display();
		
		return instance;
	}
	
	/**
	 * Initializes evm display and creates Display object for manage it.	 
	 * 
	 */
	private Display() {
		
		I2C.enable();
		
		// Clear I2C bus bug on display
		byte clearI2C[] = {(byte)0x00};
		if(I2C.write((byte)0x00, clearI2C))
			System.out.println("Display: Cleared I2C bus");
		else
			System.out.println("Display: Error on I2C bus");
		
		// Configure display
		byte conf[] = {(byte)0x38, (byte)0x39, (byte)0x14, (byte)0x74, (byte)0x54, (byte)0x6F, (byte)0x0F, (byte)0x01};
		if(I2C.writeBurst(DISPLAY_ADR, DISPLAY_CONF_REG, conf))
			System.out.println("Display: Config OK");
		else
			System.out.println("Display: Config FAILED");
		
		// Write evm hello world
		this.print(HELLO_WORLD);
	}
	
	/**
	 * Write specified string on evm display.	 
	 * @param str String to print on display.
	 */
	public void print(String str) {
		byte charArray[] = str.getBytes();
		int len = str.length();
		
		byte buf[] = new byte[16];
		
		for(int i = 0; i < ((len < 16)?len:16); i++)
			buf[i] = charArray[i];
		this.printDisplayLine(buf);
		
		if(len > 16) {
			byte buf2[] = new byte[len - 16];
			for(int j = 16; j < len; j++)
				buf2[j - 16] = charArray[j];
			
			// Switch to second line
			byte conf[] = {DISPLAY_CR_CMD};
			if(I2C.writeBurst(DISPLAY_ADR, DISPLAY_CONF_REG, conf))
				System.out.println("Display: Config OK");
			else
				System.out.println("Display: Config FAILED");
			
			this.printDisplayLine(buf2);
		}
	}
	
	private void printDisplayLine(byte line[]) {
		if(!I2C.writeBurst(DISPLAY_ADR, DISPLAY_WRITE_REG, line)) 
			System.out.println("Display: Error on write");
	}
	
	/**
	 * Clear evm display.	 
	 * 
	 */
	public void clear() {
		// Clear I2C bus (bug on display)
		byte clear[] = {DISPLAY_CLEAR_CMD};
		if(!I2C.writeBurst(DISPLAY_ADR, DISPLAY_CONF_REG, clear))
			System.out.println("Display: Error on I2C bus");
		
	}
	
}
