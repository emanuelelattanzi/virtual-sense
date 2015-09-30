/*
 *	CommandProtocol.java
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
 * platform protocol to send/receive/forward commands.
 * 
 * @author Emanuele Lattanzi
 *
 */

package javax.virtualsense.platform;

import javax.virtualsense.network.*;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.radio.Radio;
import javax.virtualsense.VirtualSense;


public class CommandProtocol extends Protocol{
		
	short lastCommandId = 0;
	short nodeId = VirtualSense.getNodeId();
	boolean iamSink = nodeId == 1?true:false; //SINK shold be always 1 ID
	
	public CommandProtocol(){
		super();
	}
	
	protected void packetHandler(Packet received){
		 if(received instanceof CommandMsg){// command message received 
			 System.out.println(" Commands msg received ");
			 if(!iamSink){
				 CommandMsg msg = (CommandMsg) received;				
				 if(msg.id != lastCommandId){ // this command has not been received
					 lastCommandId = msg.id;
					 // the first packet receiver is my best path
					 super.bestPath = msg.getSender();
					 super.notifyReceiver();
					 Thread.sleep(500);
					 super.sendBroadcast(msg);	
					 System.out.println(" forward fresh command ");
				 }		
			 }
		 }else if (received instanceof InfoMsg){ // info message to be forwarded to the sink	
			 if(!iamSink){
				 InfoMsg msg = (InfoMsg)received;
				 VirtualSense.printTime();
				 System.out.println(" Forward InfoMsg to the sink");
				 Thread.sleep(500);
				 super.send(msg);
				 Leds.setLed(Leds.LED1,false);	
			 }/*else {
				 VirtualSense.printTime();
				 super.notifyReceiver();
			 }*/
		 }
	    	
	 }
}
