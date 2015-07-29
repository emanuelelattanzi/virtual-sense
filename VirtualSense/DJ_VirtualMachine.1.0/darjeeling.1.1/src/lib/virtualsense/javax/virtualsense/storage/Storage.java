/*
 * Storage.java
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
 
 
package javax.virtualsense.storage;


/**
 * 
 * API for store on flash memory.
 * 
 * @author Matteo Dromedari
 *
 */
public class Storage
{
	public static void saveIntVar(short varId, int var) {
		_saveIntVar(Thread.getCurrentAppId(), varId, var);
	}
	
	public static int readIntVar(short varId) {
		return _readIntVar(Thread.getCurrentAppId(), varId);
	}
	
	public static void deleteVar(short varId) {
		_deleteVar(Thread.getCurrentAppId(), varId);
	}
	
	private static native void _saveIntVar(short appId, short varId, int var);
	private static native int _readIntVar(short appId, short varId);
	private static native void _deleteVar(short appId, short varId);
}