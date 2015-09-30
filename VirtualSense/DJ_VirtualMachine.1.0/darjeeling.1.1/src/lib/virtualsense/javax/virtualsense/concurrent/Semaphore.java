/*
 *	Semaphore.java

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

package javax.virtualsense.concurrent;

/**
 * A counting semaphore used for manage race condition.
 *
 * @author Lattanzi
 * 
 */
public class Semaphore 
{
	private short permits;
	private short id;
	
	private native void _waitForSemaphore(short id);
	private native void _wakeupWaitingThread(short id);
	private native short _create();
	
	
	/**
	 * Creates a Semaphore with the given number of permits.
	 * @param _permits number of permits.
	 */
	public Semaphore(short permits){
		this.permits = permits;
		this.id = _create();
	}
	
	/**
	 *  Acquires a permit from this semaphore, blocking until one is available
	 */
	public synchronized void acquire(){
		if(this.permits > 0){
			this.permits--;
		}else {
			_waitForSemaphore(this.id);
			this.permits--;
		}
	}
	
	/**
	 *  Acquires n permits from this semaphore, blocking until one is available
	 */
	public synchronized void acquire(short n){	
		this.permits -= (n - 1);
		if(this.permits > 0){
			this.permits--;
		}else {
			_waitForSemaphore(this.id);
			this.permits--;
		}
	}
		
	/**
	 *  Releases a permit, returning it to the semaphore.
	 */
	public synchronized void release(){
		this.permits++;
		if(this.permits > 0){
			_wakeupWaitingThread(this.id);
		}
	}
	
	public short availablePermits(){
		return this.permits;
	}
	
}