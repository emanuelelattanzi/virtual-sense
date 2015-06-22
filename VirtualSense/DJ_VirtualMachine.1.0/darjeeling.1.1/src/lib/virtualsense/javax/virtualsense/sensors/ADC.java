/*
 * ADC.java
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 
package javax.virtualsense.sensors;

/**
 * 
 * API for ADC (analog-to-digital) converters.
 * 
 * @author Emanuele Lattanzi
 *
 */
public class ADC
{
	public static final short CH0 		= 0;
	public static final short CH1 		= 1;
	public static final short CH2 		= 2;
	
	public static final short CHVDD 	= 4;
	
	public static final short INTREF 	= 0;
	public static final short VDDREF  	= 1;
	
	
	/**
	 * Reads a value from the an ADC channel.
	 * @param channel ADC channel. 
	 * @return measured value.
	 */
	public static short readIntRef(short channel){
		return _read(channel, INTREF, (short)0);
	}
	
	public static short readVddRef(short channel, short vdd){
		return _read(channel, VDDREF, vdd);
	}
	
	
	private static native short _read(short channel, short ref, short vdd);
}
