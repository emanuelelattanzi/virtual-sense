/*
 *	RadioTest.java
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
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */


import javax.virtualsense.network.Network;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.sensors.Temperature;
import javax.virtualsense.sensors.Humidity;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;


public class Node
{
	static short nodeId = -1;
	     
    public static void motemain()
    {   	
    	 /* slow down the system clock 
         * (normally it is configured at 10 ms)
         * to reduce power consumption 
         * leaves the CPU in the LPM3 state */        
        PowerManager.setSystemClockMillis(50);	
        Leds.setLed(0, false); 
        Leds.setLed(1, false); 
        Leds.setLed(2, false); 
        nodeId = VirtualSense.getNodeId();
        Network myNetwork = new Network(new MinPathProtocol()); 
        sender(nodeId, myNetwork);
       
            
    }    
    public static void sender(short nodeId, Network myNetwork){
    	short i = 0;
    	boolean state = true;
    	short index = 0;
    	PeopleCounter people = new PeopleCounter();
    	if(nodeId > 9) {
    		people.start();
    	}
    	while(true)
    	{    
    		System.out.print("ID: ");
    		System.out.println(nodeId);
    		DataMsg data = new DataMsg();
    		data.counter = index++;
    		data.sender_id = nodeId;
    		data.route = 0;
			data.noise = NoiseReader.read();    			
			data.co2 = ReaderCO2.read();
			if(nodeId == 7 || nodeId == 6 || nodeId ==  5 || nodeId ==  8){
				data.temp = Temperature.getValue();
				data.humid = Humidity.getValue();
				//data.light = (short)Light.getValue();
			}
			
			
			
			System.out.print(" ---- temp: ");
	   		System.out.println(data.temp);
	   		System.out.print(" ---- humid: ");
	   		System.out.println(data.humid);
	   		System.out.print(" ---- light: ");
	   		System.out.println(data.light);
	   		
	   		
			if(nodeId > 9){
				data.in = people.in;
				data.out = people.out;
			}
			if((nodeId != 3) && (nodeId != 4) && (nodeId != 5) && (nodeId != 8)){
    			data.noise = 0;
    			data.co2 = 0;   
    		}else {
    			System.out.print(" ---- MEDIA SOUND: ");
    	   		System.out.println(data.noise);
    	   		System.out.print(" ---- CO2: ");
    	   		System.out.println(data.co2);
    		}
    		Leds.setLed(0, state);        		
    		myNetwork.send(data);
    		VirtualSense.printTime();
            System.out.println(" -- SENDER packet sent");    		
    		state =! state;   
    		Thread.sleep(35000);
    		System.out.println(".");  
    		Thread.sleep(35000);
    	}          
    }
}
