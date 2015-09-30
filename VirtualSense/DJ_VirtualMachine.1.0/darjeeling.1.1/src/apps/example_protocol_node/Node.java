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
import javax.virtualsense.sensors.Temperature;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;


public class Node
{
    public static void motemain(){   
    	short i = 0;
    	boolean state = true;
    	short nodeId = VirtualSense.getNodeId();
    	
        Network myNetwork = new Network(Protocol.MINPATH);
        if(myNetwork.setPort((short)1)==Network.PORT_SET){
        	System.out.println("Port 1 set");
        }
    	
        while(true){    
    		Message msg = new Message(nodeId, 
    								  i++,
    								  Temperature.getValue());
    		Leds.setLed(Leds.LED0, state);        		
    		myNetwork.send(msg);
    		VirtualSense.printTime();
            System.out.println("Message sent to sink");    		
    		state =! state;  
    		Thread.sleep(5000);
    	}          
    }
    
}
    	
    	
    	
