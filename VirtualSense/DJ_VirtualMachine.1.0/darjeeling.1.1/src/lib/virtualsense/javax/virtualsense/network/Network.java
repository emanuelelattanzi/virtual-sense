/*
 *	Network.java
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

import javax.virtualsense.concurrent.Semaphore;
import java.util.List;
import java.util.ArrayList;

/**
 * Manages network functionalities of virtualsense as: manage a network protocol, send and receive packets from other nodes.
 * 
 * @author Lattanzi
 *
 */
public class Network {
	public static final short BUSY_PORT = -1;
	public static final short PORT_SET = 0;
	
	public static final short DEFAULT_PORT = 0;
	
	
	private static List<Short> ports = new ArrayList<Short>(); 
    private static Semaphore mutex = new Semaphore((short)1);
	
	private Protocol myProtocol;
	private short myPort;
	
	
    /**
     * Creates a network using a null comunication protocol that forwards all received packets at application layer.
     */
    public Network(){
    	init();
    	this.myProtocol = Dispatcher.registerProtocol(Protocol.NULL);    	
    }
	
	public Network(Protocol protocol){
		init();
    	this.myProtocol = protocol;
    	
    	Dispatcher.registerProtocol(protocol);
    }
    
    public Network(short sysProtocol){
    	init();
    	this.myProtocol = Dispatcher.registerProtocol(sysProtocol);    	
    }
    
    /**
     * initializes network dispatcher.
     */
    private void init(){
    	this.myPort = DEFAULT_PORT;
    	
        Dispatcher.launch(); // if already running does nothing 
        Thread.yield();
    }
    
    public void setSink(){
    	this.myProtocol.setSink();
    } 

    /**
     * Sends a packet to an another node following the rules of protocol.
     * @param packet to be sent.
     */
    public void send(Packet packet){
    	packet.port = this.myPort;
    	System.out.print(this.myProtocol.toString());System.out.print("(on port: ");System.out.print(this.myPort);System.out.print("): ");
    	Dispatcher.send(packet, this.myProtocol);	
    }
    
    /**
     * Sends a packet to a neighboring node.
     * @param packet to be sent.
     * @param destination node ID.
     */
    public void sendTo(Packet packet, short nodeId){
    	packet.port = this.myPort;
    	Dispatcher.sendTo(packet, this.myProtocol, nodeId);
    }

    /**
     * Waits to receive a packet from an another network node. 
     * @param received packet.
     */
    public Packet receive(){
    	Packet p;
    	do{
    		p = Dispatcher.receive(this.myProtocol);
    		System.out.print(this.myProtocol.toString());System.out.print(" incoming packet on port: ");System.out.println(p.port);
    	}
    	while(p.port != myPort);
    		
        return p;
    }  
      
    public short setPort(short port){
    	mutex.acquire();
    	
    	short ret = BUSY_PORT;
    	boolean free = true;
    	
    	for(short j = (short)0; j<ports.size() && free; j++){
    		if(ports.get(j).shortValue() == port){
    			free = false;
    		}
    	}
    	
    	if(free){
    		ports.add(new Short(port));
    		this.myPort = port;
    		ret = PORT_SET;
    	}
    	mutex.release();
    	
    	return ret;
    }
    
}
