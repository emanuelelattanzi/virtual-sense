/*
*	MinPathProtocol.java
 * 
 *	Copyright (c) 2012 DiSBeF, University of Urbino.
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
package javax.virtualsense.network.protocols.minpath;


/**
 * Simple Radio Test Application.
 * 
 * @author Emanuele Lattanzi
 *
 */
import javax.virtualsense.network.*;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.radio.Radio;
import javax.virtualsense.VirtualSense;


public class MinPathProtocol extends Protocol{
		
	private short minHops = 127;	
	private short epoch = -1;
	private InterestSender interSender;
	short nodeId = VirtualSense.getNodeId();
	
	public MinPathProtocol(){
		super();
		this.interSender = new InterestSender(this);
	}
	
	protected void packetHandler(Packet received){
		
		if(sink){
			// SINK
			if(received instanceof InterestMsg){// INTEREST MESSAGE 
				 System.out.println(" received interest ");
				 //NOTINGH TO DO; IS A PONG INTEREST
			 }
			 if (received instanceof DataMsg){ //DATA	
				 super.notifyReceiver();		 
			 }	 	
		}else {
			// NODES
			if(received instanceof InterestMsg){// INTEREST MESSAGE 
				System.out.print("NETWORK - received interest ");
				System.out.println(received.getSender());
				InterestMsg msg = (InterestMsg) received;
				Leds.setLed(Leds.LED1,true);
				
				if(msg.epoch > epoch){ // new epoch start -- reset routing table
					//if(msg.epoch > epoch && msg.hops > 0){ // new epoch start -- reset routing table
					epoch = msg.epoch;
					super.bestPath = -1;
					minHops = (byte)127;
				}
				
				if(msg.hops < this.minHops ){ 
					//if(msg.hops < this.minHops && msg.hops > 0){ //&& msg.hops > 0 to force route
					VirtualSense.printTime();				 
					this.minHops = msg.hops;
					super.bestPath = msg.nodeID;
					System.out.print("NETWORK - routing updated to ");
					System.out.println(super.bestPath);
					msg.hops+=1;				 
					msg.nodeID = nodeId;
					Thread.sleep(700);
					super.sendBroadcast(msg);
					System.out.println("NETWORK - interest Forwarded");
					Leds.setLed(Leds.LED1,false);
					
				}	
				//Leds.setLed(Leds.LED2,false);
			}
			if (received instanceof DataMsg){ //DATA	
				DataMsg msg = (DataMsg)received;
				Leds.setLed(Leds.LED2,true);
				VirtualSense.printTime();
				if(super.bestPath > 0){
					System.out.print("NETWORK - forward packet to the sink ");
					System.out.print(msg.sender_id);
					System.out.print(" ");
					System.out.println(received.getSender());
					msg.route =(short)( msg.route + ((short)(0x01 << nodeId)));
					Thread.sleep(1700);
					super.send(msg);
					Leds.setLed(Leds.LED1,false);
				}else {
					System.out.print("NETWORK - dropped broadcast data!");
					System.out.print(msg.sender_id);
					System.out.print(" ");
					System.out.println(received.getSender());
				}
			}
    	}
	}
	
	protected void sinkInit(){
		this.interSender.start();
	}
	
	public String toString(){
    	return "MinPathProtocol";
    }
	
}

