/*
 *      InterestSender.java
 * 
 *      Copyright (c) 2013 DiSBeF, University of Urbino.
 * 
 *      This file is part of VirtualSense.
 * 
 *      VirtualSense is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      VirtualSense is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 * 
 *      You should have received a copy of the GNU General Public License
 *      along with VirtualSense.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Interest sender of min path protocol.
 * 
 * @author Emanuele Lattanzi
 *
 */


import java.lang.Thread;
import javax.virtualsense.network.*;
//import javax.virtualsense.network.Protocol;
import javax.virtualsense.VirtualSense;
//import javax.virtualsense.network.protocols.minpath.MinPathProtocol;
//import javax.virtualsense.network.protocols.none.NullProtocol;
import javax.virtualsense.actuators.Leds;

public class Receiver extends Thread{
	
	private Network n;
	private short nodeID;
	
    public Receiver(Network nt, short id){
    	this.n = nt;
    	this.nodeID = id;
    }
    
    public void run(){                                  
        while(true){
       
            	Packet p = n.receive();
            	
            	short id = p.getSender();
            	
            	if(p instanceof Message){
            		Message m = (Message)p;
            		
            		switch(id){
            			case 1:{
            				if(m.LED0 == 1)
                    			Leds.setLed(Leds.LED0, true);
                    		else
                    			Leds.setLed(Leds.LED0, false);
            			}
            			break;
            			case 2:{
            				if(m.LED1 == 1)
                    			Leds.setLed(Leds.LED1, true);
                    		else
                    			Leds.setLed(Leds.LED1, false);
            			}
            			break;
            			case 3:{
            				if(m.LED2 == 1)
                    			Leds.setLed(Leds.LED2, true);
                    		else
                    			Leds.setLed(Leds.LED2, false);
            			}
            			break;
            			default:
            			break;
            		}
            		
            	}
        }
    }
}
            		
          
            		
            		
            		
            		
            	