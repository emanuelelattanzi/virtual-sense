/*
 *	VirtualSense.java
 * 
 *  Copyright (c) 2011 DiSBeF, University of Urbino.
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

package javax.virtualsense;

/**
 * The VirtualSense system wrapper class.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class VirtualSense
{
	/**
	 * Returns the system time in seconds.
	 */
    public static int getSecond(){
		return (System.currentTimeMillis()/1000);
	}
    
    /**
	 * Returns the system time in minutes.
	 */
	public static int getMinute(){
		return (System.currentTimeMillis()/60000);
	}
	
	/**
	 * Returns the system time in hours.
	 */
	public static int getHour(){
		return (System.currentTimeMillis()/3600000); 	
	}
	
	/**
	 * Returns the identification of current node.
	 */
	public static native short getNodeId();
	
	/**
	 * Burn on eeprom the identification of current node.
	 */
	public static native void setNodeId(short nodeId);
	
	/**
	 * Write on serial port the system time.
	 */
	public static native void printTime();
}
