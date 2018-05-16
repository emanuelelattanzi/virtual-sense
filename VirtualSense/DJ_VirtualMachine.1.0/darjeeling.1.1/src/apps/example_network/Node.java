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
import java.lang.Thread;


public class Node extends Thread
{
	/**
	 * WARNING: Set different node id for each programed node!
	 */
	private static final short ID_TO_SEND = 3;
	
	private Network network;
	
	
	public Node(Network network){
		this.network = network;
	}
	
	
    public static void motemain() {
    	
    	short i = 0;
    	short nodeId = VirtualSense.getNodeId();
    	System.out.print("MAIN: node id ");System.out.println(nodeId);
    	System.out.print("MAIN: node id to send ");System.out.println(ID_TO_SEND);
    	
    	
        Network myNetwork = new Network();
        if(myNetwork.setPort((short)2)==Network.PORT_SET){
        	System.out.println("Port 2 set");
        }
        Node node = new Node(myNetwork);
        node.start();
    	
        // SENDER BEHAVIOR
        while(true){   
        	Leds.setLed(Leds.LED0, true);
    		Message msg = new Message(nodeId, 
    								  i++,
    								  Temperature.getValue());
    		
    		/*myNetwork.sendTo(msg, ID_TO_SEND);
    		VirtualSense.printTime();
            System.out.print(" SENDER: message sent to ");System.out.println(ID_TO_SEND);   	*/	
    		Leds.setLed(Leds.LED0, false);
    		
    		Thread.sleep(1000);
    	}          
    }
    
    public void run(){
    	// RECEIVER BEHAVIOR
        while(true){
        	
        	Message msg = (Message)this.network.receive();
        	
        	Leds.setLed(Leds.LED1, true);
    		VirtualSense.printTime();
    		System.out.print(" RECEIVER: received message from ");
    		System.out.print(msg.getSender());System.out.print(" its temperature: ");
    		System.out.println(msg.value);
    		Thread.sleep(200);
    		Leds.setLed(Leds.LED1, false);        		
    	}
    }
    
}
    	
    	
    	
