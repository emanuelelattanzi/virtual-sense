/*
 *	Protocol.java
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
import javax.virtualsense.radio.Radio;

/**
 * Defines abstract behavior of the network. Must be redefined to implement a network protocol.
 *
 * @author Lattanzi
 */
public abstract class Protocol 
{
    public static final short NULL = 0;
	public static final short MINPATH = 1;
	
    protected short bestPath;
    private  boolean running; 
    private  Packet actualPacket;
    private  Semaphore packet;
    protected boolean sink;
    
    public Protocol(){
    	this.bestPath =-1;
        this.running = true;
        this.actualPacket = null;
        this.packet = new Semaphore((short)0);
        this.sink = false;
    }
    /**
     * Send a packet to a node inside the network choosing the best path.
     * @param packet to be sent.
     */
    // invoke Radio.send(short, data) with bestPath as dest
    //protected void send(Packet p){ //METH for invoque send in class InterestMsg of minPat package 
    public void send(Packet p){
    	if (p instanceof UnicastPacket){
    		if(bestPath >= 0){
    			System.out.print("send unicast to ");
    			System.out.println(bestPath);
    			Radio.send(bestPath, p.toByteArray());     
    		}else{
    			System.out.println("send broadcast");
    			Radio.broadcast(p.toByteArray());
            
    		}
    	}else if (p instanceof BroadcastPacket){
    		System.out.println("send broadcast packet");
    		Radio.broadcast(p.toByteArray());
    	}else {
    		System.out.println("send broadcast packet 2");
    		Radio.broadcast(p.toByteArray());
    	}
    }
    
    protected void sendUnicast(short destID, Packet p){
    	Radio.send(destID, p.toByteArray());     
    }
    
    
    /**
     * Send a broadcast packet.
     * @param packet to be sent.
     */
    protected void sendBroadcast(Packet p){
    	System.out.println("send broadcast packet method");
    	Radio.broadcast(p.toByteArray());
    }
    
    /**
     * Waits to received a packet from an another network node.
     * 
     * @return received packet.
     */
    protected Packet receive(){     
       packet.acquire();        
       return actualPacket;
    }
    
    /**
     * Wake-up the thread waiting to receive an incoming packet, blocked by the receive() method.
     */
    protected void notifyReceiver(){
        // should wake-up the receiver calling thread!
        // packet.release();
    	
    	// wake-up all threads using this protocol!
    	do{
    		this.packet.release();
    	}while(packet.availablePermits() < 0);
    }
    
    /**
     * To redefine for implement protocol functionality. 
     * This method is invoche by thread generated on method start() each time there is an incoming packet. 
     * @param packet to be handled.
     */
    protected void packetHandler(Packet p){
    	
    }
    
    protected void setSink(){
    	this.sink = true;
    	this.sinkInit();
    }
    
    protected void sinkInit(){

    }
    	    
    /**
     * Stop the network.
     */
    protected void stop(){
        running = false;
    }
    
    protected void setActualPacket(Packet p){
    	this.actualPacket = p;
    }
     
}
