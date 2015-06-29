/*
 *	Blink.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
import javax.virtualsense.actuators.Leds;

import javax.virtualsense.sensors.*;
import javax.virtualsense.sensors.evm.accelerometer.*;
import javax.virtualsense.VirtualSense;
import javax.darjeeling.Darjeeling;

public class SenseMultiUser
{
    public static void motemain()
    {
    	Accelerometer acc = new Accelerometer();
    	
        while(true)
        {
        	AccVector accVector = acc.getValue();
        	if(accVector != null) {
        		System.out.println("Acceleration vector:");
        		System.out.print("  x: ");System.out.print(accVector.x);
        		System.out.print("  y: ");System.out.print(accVector.y);
        		System.out.print("  z: ");System.out.println(accVector.z);
        	}
        	Thread.sleep(500);
        	
        	/*System.out.print("ADC0: ");
        	System.out.print(ADC.readIntRef(ADC.CH0));
        	System.out.println("mV");
        	Thread.sleep(1000);

        	System.out.print("ADC1: ");
        	System.out.print(ADC.readIntRef(ADC.CH1));
        	System.out.println("mV");
        	Thread.sleep(1000);
        	
        	System.out.print("ADC2: ");
        	System.out.print(ADC.readIntRef(ADC.CH2));
        	System.out.println("mV");
        	Thread.sleep(1000);
        	
        	System.out.print("Ext. humidity: ");
        	System.out.print(Humidity.getValue());
        	System.out.println(" %");
        	Thread.sleep(1000);
        	        	
        	System.out.print("Ext. temp: ");
        	System.out.print(Temperature.getValue());
        	System.out.println(" mC");
        	Thread.sleep(1000);
        	        	
        	System.out.print("Board temp: ");
        	System.out.print(Temperature.getBoardValue());
        	System.out.println("mC");
        	Thread.sleep(1000);
        	
        	System.out.print("Current Vdd: ");
        	System.out.print(ADC.readIntRef(ADC.CHVDD));
        	System.out.println("mV");
        	Thread.sleep(1000);*/
        }
    }
}
