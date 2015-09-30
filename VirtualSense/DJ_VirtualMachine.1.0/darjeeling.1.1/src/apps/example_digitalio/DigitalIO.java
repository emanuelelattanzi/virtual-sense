/*
 *	DigitalIO.java
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
 * Simple Interrupt and digital input test application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.digitalio.*;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.actuators.evm.display.*;
import java.lang.Runtime;
import javax.virtualsense.VirtualSense;

public class DigitalIO
{
	static int  temp = 0;
    public static void motemain() {
    	
	    DigitalPin dio0 = new DigitalPin(true, DigitalPin.DIO0);
	    DigitalPin dio1 = new DigitalPin(true, DigitalPin.DIO1);
	    DigitalPin dio2 = new DigitalPin(true, DigitalPin.DIO2);
	    DigitalPin dio3 = new DigitalPin(true, DigitalPin.DIO3);
	    
	    Button btn0 = new Button(InterruptPin.INT0, Leds.LED0);
	    Button btn1 = new Button(InterruptPin.INT1, Leds.LED1);
	    Button btn2 = new Button(InterruptPin.INT2, Leds.LED2);
	    btn0.start();
	    btn1.start();
	    btn2.start();
	    
	    
	    while(true)
	    {        	    	
	    	if(dio0.read()) 
	    		System.out.print("DIO0:up ");
	    	else
	    		System.out.print("DIO0:down ");
	    	
	    	if(dio1.read()) 
	    		System.out.print("DIO1:up ");
	    	else
	    		System.out.print("DIO1:down ");
	    	
	    	if(dio2.read()) 
	    		System.out.print("DIO2: up ");
	    	else
	    		System.out.print("DIO2: down ");
	    	
	    	if(dio3.read()) 
	    		System.out.print("DIO3: up ");
	    	else
	    		System.out.print("DIO3: down ");   
	
	 		VirtualSense.printTime();
	        System.out.print(" Free mem: ");
	        System.out.println(Runtime.freeMemory());
	        
	        Thread.sleep(1000);
	    }
    }
}