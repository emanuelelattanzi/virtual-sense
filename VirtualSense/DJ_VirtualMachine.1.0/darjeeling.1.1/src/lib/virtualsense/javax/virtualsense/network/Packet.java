/*
 *	Packet.java
 * 
 *      Copyright (c) 2012 DiSBeF, University of Urbino.
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
package javax.virtualsense.network;

import java.io.Serializable;

/**
 * Defines the format of packets exchanged from nodes.
 *
 * @author Lattanzi
 */
public class Packet implements Serializable
{
	 
     private short s_ID;
     public short port = (short)0;
     
     public Packet(){

     }
     
     public Packet(short senderId){
    	 this.s_ID = senderId;
     }
     
     /* this constructor is used to create a packet from a received byte array taken from the radio */
     public static native Packet createPacket(byte data[]);
     public native byte[] toByteArray(); 
     
     /**
      * Returns sender identifier.
      * @returns sender of packet.
      */
     public short getSender(){
         return this.s_ID;
     }
        
     /**
      * Set sender identifier of current packet.
      * @param new packet sender.
      */
     protected void setSender(short sender){
    	 this.s_ID = sender;
     }     
     
}
