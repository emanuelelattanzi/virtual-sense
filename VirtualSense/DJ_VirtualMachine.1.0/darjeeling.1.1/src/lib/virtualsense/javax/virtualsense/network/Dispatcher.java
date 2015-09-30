/*
 *	Dispatcher.java
 * 
 *      Copyright (c) 2013 DiSBeF, University of Urbino.
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

import java.lang.Thread;
import javax.virtualsense.radio.Radio;
import javax.virtualsense.network.protocols.minpath.MinPathProtocol;
import javax.virtualsense.network.protocols.none.NullProtocol;
import javax.virtualsense.concurrent.Semaphore;

/**
 * Defines abstract behavior of the network. Must be redefined to implement a network protocol.
 *
 * @author Lattanzi
 */
public class Dispatcher extends Thread
{
    private static boolean running = false;
    //private static ProtocolPacket protocolPackets[] = new ProtocolPacket[5]; //TODO dynamic allocation of the structure
    private static Protocol protocols[] = new Protocol[10]; //TODO dynamic allocation of the structure
    private static int index = 0;
    private static Dispatcher dispatcherThread = null;
    private static Semaphore mutex = new Semaphore((short)0);
    
    
    private Dispatcher(){
    	mutex.release();
    }
      
      /**
     * Start the network dispatcher.
     */
    protected static void launch(){   
    	if(!running){
    		running = true;    	
	        dispatcherThread = new Dispatcher();
	        dispatcherThread.start();
    	}
    }
    public void run(){
    	short s_id = -1;
    	Radio.init(); // initilize Radio and pass this as receiving thread
    	//System.out.println("Starting Dispatcher");
    	
    	while(running){
    		byte d[] = Radio.receive(); // waiting for a packet
    		s_id = Radio.getSenderId();        			 
    		Packet p = Packet.createPacket(d);
    		if(p != null){
    			p.setSender(s_id);
    			// look for registered packets/protocols and invoke related packetHandler
    			for(int i = 0; i < index; i++){
    				if(protocols[i] != null){
    					protocols[i].setActualPacket(p);
    					protocols[i].packetHandler(p);
    				}
    			}
    		}
    	}           
    }
	
    protected static void send(Packet p, Protocol protocol){
    	//System.out.println("Invoco la ned del protocol");
	    protocol.send(p); // TODO: look for the protocol to kinow if it has been registered
    }
    protected static void sendTo(Packet p, Protocol protocol, short nodeID){
	    protocol.sendUnicast(nodeID, p); // TODO: look for the protocol to kinow if it has been registered
    }
    
    // application level receive
    protected static Packet receive(Protocol protocol){
    	Packet received;// TODO: look for the protocol to know if it has been registered
		received = protocol.receive();
		return received;
    }
    
    /**
     * Stop the network.
     */
    protected void stop(){
        running = false;
    }
    
    protected static void registerProtocol(Protocol protocol){
    	mutex.acquire();
		protocols[index] = protocol;
    	System.out.print("Registered protocol[");System.out.print(index);System.out.print("]: ");System.out.println(protocols[index].toString());
    	//System.out.print(index);
    	index = index +1;
    	mutex.release();
    }
    
    protected static Protocol registerProtocol(short sysProtocol){
    	boolean find = false;
    	Protocol p = null;
    	mutex.acquire();
    	// Check if sysProtocol is already registered
    	for(int i = 0; i < index && !find; i++){
    		switch(sysProtocol){
    			case (short)0: {
					if(protocols[i] instanceof NullProtocol){
						find = true;
						p = protocols[i];
					}
    			}
    			break;
    						
    			case (short)1: {
					if(protocols[i] instanceof MinPathProtocol){
						find = true;
						p = protocols[i];
					}
    			}
    			break;			
    						
    			//default: break;
    		}
    	}
    	
    	// If not already registered, register a new protocol
    	if(!find){
    		switch(sysProtocol){
				case (short)0: {
					p = new NullProtocol();
					//System.out.println("Null protocol created");
				}
				break;
				case (short)1: {  
					p = new MinPathProtocol();
					//System.out.println("Min path protocol created");
				}
				break;
				//default: break;
    		}
				
    		protocols[index] = p;
    		System.out.print("Registered protocol[");System.out.print(index);System.out.print("]: ");System.out.println(protocols[index].toString());
        	//System.out.println(index);
        	index = index +1;
    	}
    	mutex.release();
    	
    	return p;
    }
     
}
