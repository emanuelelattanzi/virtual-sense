/*
 *	BurnNodeId.java
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

/**
 * Id burn Application.
 * 
 * @author Matteo Dromedari
 *
 */
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.VirtualSense;
import java.lang.Runtime;

public class BurnNodeId
{
	private static final short NODEID = 2;
	
    public static void motemain(){
    	
    	Thread.sleep(500);
        System.out.println("Burn node id...");
        
        VirtualSense.setNodeId(NODEID);
        
        Leds.setLed(2, true);
    	System.out.print("New node id is: ");
    	System.out.println(VirtualSense.getNodeId());
      
    	while(true){
    		Thread.sleep(500);
    	}
    }
    
}