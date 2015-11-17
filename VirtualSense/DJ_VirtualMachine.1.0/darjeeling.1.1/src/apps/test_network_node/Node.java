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


import javax.virtualsense.network.*;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.sensors.Light;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;
import javax.virtualsense.digitalio.DigitalPin;


public class Node
{
    public static void motemain(){   
    	short i = 0;
    	short state = 0;
    	short nodeId = VirtualSense.getNodeId();
    	
        Network myNetwork = new Network(Protocol.NULL);
        if(myNetwork.setPort((short)1)==Network.PORT_SET){
        	System.out.println("Port 1 set");
        }
        
        Receiver r = new Receiver(myNetwork, nodeId);
        r.start();
        
        Leds.setLed(Leds.LED0, false);
    	Leds.setLed(Leds.LED1, false);
    	Leds.setLed(Leds.LED2, false);
    	
        //DigitalPin dio = new DigitalPin(true, DigitalPin.DIO0);

        while(true){ 
        	
        	short led0 = 0;
        	short led1 = 0;
        	short led2 = 0;
        	
        	short light = Light.getValue();
        	System.out.println(light);
        	if(light<(short)500){
        		Leds.setLed((nodeId-1), true);
        		led0 = (nodeId == 1)?(short)1:(short)0;
        		led1 = (nodeId == 2)?(short)1:(short)0;
        		led2 = (nodeId == 3)?(short)1:(short)0;
        	}else{
        		Leds.setLed((nodeId-1), false);
        	}
        	
    		Message msg = new Message(led0, led1, led2);
    		msg.setSender(nodeId);
    		
    		//Leds.setLed(0, state);        		
    		myNetwork.send(msg);
    		VirtualSense.printTime();
            System.out.println("Message sent to sink");    		
    		//state =! state;  
    		Thread.sleep(300);
    	}          
    }
    
}
    	
    	
    	
