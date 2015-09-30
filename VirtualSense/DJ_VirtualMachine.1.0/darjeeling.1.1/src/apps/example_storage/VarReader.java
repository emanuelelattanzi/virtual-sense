/*
 *	Storage.java
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

import javax.virtualsense.powermanagement.PowerManager;
import javax.virtualsense.storage.Storage;
import java.lang.Runtime;

/**
 * Storage application.
 * 
 * @author Matteo Dromedari
 *
 */
public class VarReader
{
	private static final short TEST_VAR = (short) 2;
	
	public static void motemain()
    {
		int testVar = 12345;

		System.out.print("Read TEST_VAR: ");
		System.out.println(Storage.readIntVar(TEST_VAR));
		
		System.out.print("Save TEST_VAR: ");
		System.out.println(testVar);
		
		Storage.saveIntVar(TEST_VAR, testVar);
		
		System.out.print("Read TEST_VAR: ");
		System.out.println(Storage.readIntVar(TEST_VAR));
    }
}