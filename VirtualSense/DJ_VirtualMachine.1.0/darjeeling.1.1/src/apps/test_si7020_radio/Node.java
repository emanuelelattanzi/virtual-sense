/*
 *	Node.java
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
 * Simple Radio example for measure temperature, pressure and humidity.
 * All nodes send its samples to the sink (ID = 1) that writes data on serial port.
 * 
 * @author Matteo Dromedari
 *
 */


import javax.virtualsense.network.*;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.sensors.Temperature;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;
import java.lang.Thread;
import javax.virtualsense.digitalio.bus.I2C;
import javax.virtualsense.sensors.*;


public class Node extends Thread
{
	/**
	 * WARNING: Set different node id for each programed node!
	 */
	private static final short NODE_ID = 3;
	private static final short SYNK_ID = 1;
	private static final boolean LOW_POWER = true;
	
	
	private Network network;
	
	
	public Node(Network network){
		this.network = network;
	}
	
    public static void motemain(){
    	
    	short i = 0;
    	short nodeId = VirtualSense.getNodeId();
    	if(nodeId != NODE_ID)
    		VirtualSense.setNodeId(NODE_ID);
    	
        Network myNetwork = new Network();
        if(myNetwork.setPort((short)2)==Network.PORT_SET){
        	//System.out.println("Port 2 set");
        }
        Node node = new Node(myNetwork);
        SI7020 si7020 = new SI7020();
        
        // Sensor init
        I2C.enable();
   	 	Thread.sleep(1000);
   	 	System.out.println("_NodeID_Temp(cC)_Pressure(hPa)_Humidity(%cRH)");
   	 	short temp = (short)0;
   	 	short pres = (short)0;
   	 	short hum = (short)0;
   	 	if(LOW_POWER)
   	 		Leds.setLed(Leds.LED0, false);
   	 	else
   	 		Leds.setLed(Leds.LED0, true);
   	 	Leds.setLed(Leds.LED1, false);
   	 	Leds.setLed(Leds.LED2, false);
        
        node.start();
    	
        // SENDER BEHAVIOR
        while(true){ 
        	
        	if(!LOW_POWER)
        		Leds.setLed(Leds.LED1, true);
    		Message msg = new Message(nodeId, 
    								  i++);
    		msg.pres = Pressure.getValue();
        	msg.temp = si7020.readTemp();
        	msg.hum = si7020.readHum();
    		
    		if(nodeId != SYNK_ID)
    			myNetwork.sendTo(msg, SYNK_ID);
    		System.out.print("_");System.out.print(nodeId);System.out.print("_");
        	System.out.print(msg.temp);System.out.print("_");
        	System.out.print(msg.pres);System.out.print("_");
        	System.out.println(msg.hum);
        	if(!LOW_POWER)
        		Leds.setLed(Leds.LED1, false);
    		
    		Thread.sleep(75500);
    	}          
    }
    
    public void run(){
    	// RECEIVER BEHAVIOR
        while(true){
        	
        	Message msg = (Message)this.network.receive();
        	
        	if(!LOW_POWER)
        		Leds.setLed(Leds.LED0, false);
        	System.out.print("_");System.out.print(msg.getSender());System.out.print("_");
        	System.out.print(msg.temp);System.out.print("_");
        	System.out.print(msg.pres);System.out.print("_");
        	System.out.println(msg.hum);
        	if(!LOW_POWER)
        		Leds.setLed(Leds.LED0, true);        		
    	}
    }
    
}
    	
    	
    	
