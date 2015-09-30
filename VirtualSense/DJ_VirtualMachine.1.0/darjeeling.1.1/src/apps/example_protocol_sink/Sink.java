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
import javax.virtualsense.network.*;
import javax.virtualsense.network.protocols.minpath.*;
import javax.virtualsense.network.Packet;
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.VirtualSense;


/**
 * Simple Radio Test Application application.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class Sink
{	     
    public static void motemain()
    {
    	//VirtualSense.setNodeId((short)1);
    	//System.out.println("Set node id to 1 for be the Sink");
    	
    	Network myNetwork = new Network(Protocol.MINPATH);
        if(myNetwork.setPort((short)1)==Network.PORT_SET){
        	System.out.println("Port 1 set");
        }
    	myNetwork.setSink();
    	
    	while(true){
    		System.out.println("Wait for packets...");
        	Packet p = myNetwork.receive();
        	
        	System.out.println("<packet>");
        	System.out.print("   >time: ");
        	System.out.println(System.currentTimeMillis());
        	System.out.print("   >router: ");
        	System.out.println(p.getSender());        	
        	if(p instanceof Message){
        		Message m = (Message)p;
        		System.out.print("   >sender: ");
        		System.out.println(m.sender_id);
        		System.out.print("   >counter: ");
        		System.out.println(m.counter);
        		System.out.print("   >route: ");
        		System.out.println(m.route);
        		System.out.print("   >value: ");
        		System.out.println(m.value);
        	}else {
        		System.out.println("UNKNOWN");
        	}
        	System.out.println("</packet>\n");
  	    }
    }
    
}





