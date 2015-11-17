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
	
	public static byte SI7020_ADDRESS = (byte)0x80;
	public static byte SI7020_READ_TEMP_NOHOLD = (byte)0xF3;
	public static byte SI7020_READ_HUMID_NOHOLD = (byte)0xF5;
	public static byte SI7020_READ_USER_REG_1 = (byte)0xE7;
	public static byte SI7020_WRITE_USER_REG_1 = (byte)0xE6;
	public static byte SI7020_RESET = (byte)0xFE;
	
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
        
        // Sensor init
        I2C.enable();
   	 	Thread.sleep(1000);
   	 	System.out.println("_NodeID_Temp(mC)_Pressure(hPa)_Humidity(%mRH)");
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
        	msg.temp = readTempSI7020();
        	msg.hum = readHumSI7020();
    		
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
    
	public static short readTempSI7020() {
    	
    	short temp = (short)0;
    	 
    	 I2C.start();
    	 I2C.write(SI7020_ADDRESS);
    	 I2C.write(SI7020_READ_TEMP_NOHOLD);
    	 
    	 do {
    		 System.out.print("");
    		 I2C.start();
    	 }
    	 while(!I2C.write((byte)(SI7020_ADDRESS | 1)));

    	 byte b1 = I2C.read(true);
    	 //System.out.print("b1: ");
    	 //System.out.print(b1);
    	 System.out.print("");
    	 byte b2 = I2C.read(false);
    	 //System.out.print(" b2: ");
    	 //System.out.println(b2);
    	 
    	 I2C.stop();
    	 
    	 int read = ((int)b1 << 8) & 0x0000FF00;
    	 read |= (int)b2;
    	 
    	 temp = (short)(((read * 21965)/ 81920)-4685);
    		
    	 return temp;
    }
	
	
	public static short readHumSI7020() {
    	
    	short hum = (short)0;
    	 
    	 I2C.start();
    	 I2C.write(SI7020_ADDRESS);
    	 I2C.write(SI7020_READ_HUMID_NOHOLD);
    	 
    	 do {
    		 System.out.print("");
    		 I2C.start();
    	 }
    	 while(!I2C.write((byte)(SI7020_ADDRESS | 1)));

    	 byte b1 = I2C.read(true);
    	 //System.out.print("b1: ");
    	 //System.out.print(b1);
    	 System.out.print("");
    	 byte b2 = I2C.read(false);
    	 //System.out.print(" b2: ");
    	 //System.out.println(b2);
     	 
    	 I2C.stop();

    	 int read = ((int)b1 << 8) & 0x0000FF00;
    	 read |= ((int)b2 & 0x0000FC);
    	 
    	 hum = (short)(((read * 12500)/ 65536)-600);
    		
    	 return hum;
    }
    
}
    	
    	
    	
