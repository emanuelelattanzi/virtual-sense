/*
 *	Button.java
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
 * Simple button Interrupt handler.
 * 
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.digitalio.*;

public class Button extends Thread
{
	private InterruptPin intPin;
	private short led;
	
	
    public Button(short pin, short led)
    {
    	this.intPin = new InterruptPin(true, pin, InterruptPin.PULL_UP);
    	this.led = led;
    }
  
    public void run()
    {	
    	boolean state = false;
    	Leds.setLed(this.led, state);

    	while(true)
    	{   
    		// Wait for falling edge interrupt
    		this.intPin.waitForInterrupt();
    		
    		state = !state;
    		Leds.setLed(this.led, state);
    		
    		// Debounce delay 
    		Thread.sleep(200);
    	}
    }
}