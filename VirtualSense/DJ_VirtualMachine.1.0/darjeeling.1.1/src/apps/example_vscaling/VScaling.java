/*
 *	VScaling.java
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
 * Simple Application to test the MCU frequency and V scaling 
 * functions of the PowerManager.
 * 
 * @author Emanuele Lattanzi
 *
 */


import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.actuators.Leds;

public class VScaling
{
	public static void motemain()
    {
                       
        /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(1000);
        //while(true)
        //{
        	Thread.sleep(3000); 
        	PowerManager.setMCUFrequency(PowerManager.MCU_25MHZ);
			System.out.println("25MHZ: ");
			CPUBurst();    			
			Thread.sleep(1000); 
			
			PowerManager.setMCUFrequency(PowerManager.MCU_20MHZ);
			System.out.println("20MHZ: ");
			CPUBurst();
			Thread.sleep(1000); 
			
			PowerManager.setMCUFrequency(PowerManager.MCU_16MHZ);
			System.out.println("16MHZ: ");
			CPUBurst();
			Thread.sleep(1000); 
			
			PowerManager.setMCUFrequency(PowerManager.MCU_12MHZ);
			System.out.println("12MHZ: ");
			CPUBurst();
			Thread.sleep(1000); 
			
			PowerManager.setMCUFrequency(PowerManager.MCU_8MHZ);  
			System.out.println("8MHZ: ");
			CPUBurst();
			Thread.sleep(1000); 
			
			PowerManager.setMCUFrequency(PowerManager.MCU_4MHZ);  
			System.out.println("4MHZ: ");
			CPUBurst();
			Thread.sleep(1000); 			
    		
			Leds.setLed(Leds.LED0,true);              
			Leds.setLed(Leds.LED1,true);
			Leds.setLed(Leds.LED2,true);

        //}
    }
    public static void CPUBurst(){
    	for(int i = 0; i < 10000; i++);
    }
}
