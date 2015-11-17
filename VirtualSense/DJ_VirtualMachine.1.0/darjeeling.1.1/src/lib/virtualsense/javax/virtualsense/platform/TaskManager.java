/*
 *	TaskManager.java
 * 
 *      Copyright (c) 2013 DiSBeF, University of Urbino.
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
package javax.virtualsense.platform;

import javax.virtualsense.VirtualSense;
import javax.virtualsense.network.Network;
import java.util.ArrayList;
import java.util.Iterator;

/**
 *
 * @author Lattanzi
 */
public class TaskManager {
	//private static ArrayList<Task> tasks = new ArrayList<Task>((short)20);
	
	private static Task tasks[] = new Task[20];
	private static int index = 0;
	
	protected static void initDefaultTasks(){
		short tn = Task.getDefaultTasksNumber();
		for(short i = 0; i < tn; i++){
			short executionContextID = Task.getDefaultExecutionContextID(i);
			if(Task.defaultNeedToLoad(i))
				loadTask(executionContextID);
			if(Task.defaultNeedToStart(i))
				startTask(executionContextID);
		}
	}
	
    protected static void loadTask(short executionContextID){
        // create task using a native method and push reference on the stack
    	//---System.out.print("----- Load a new app!!!!  id ");System.out.println(executionContextID);
        addTask(new Task(executionContextID));    
        Thread.yield(); // needed to allow deferred infusion initialization
    }
    
    
    protected static void startTask(short executionContextID){
    	//---System.out.println("----- Starting a task ");
    	Task toStart = getTask(executionContextID);
    	if(toStart != null)
    		toStart.startExecution();        
    }
    
    protected static void stopTask(short executionContextID){
    	//---System.out.println("----- Stopping a task ");
    	Task toStop = getTask(executionContextID);
    	if(toStop != null)
    		toStop.stopExecution();        
    }
    
    protected static void unloadTask(short executionContextID){
    	//System.out.print("----- Unloading task ");
    	Task toUnload = getTask(executionContextID);
    	if(toUnload != null)
    		toUnload.unload();        
    }
    protected static void sendInfoTask(short executionContextID, Network network){
    	
    	Task toSendInfo = getTask(executionContextID);
    	if(toSendInfo != null){
    		InfoMsg msg = new InfoMsg();
    		msg.nodeID = VirtualSense.getNodeId();
    		msg.executionContextID = executionContextID;
    		msg.loaded = toSendInfo.loaded;
    		msg.running = toSendInfo.running;
    		Thread.sleep(1500);
    		network.send(msg);
    		//---System.out.println("----- Send info mesg done");
    	}else {
    		//Task.sendGlobalInfo();
    		
    	}
    }
    
    protected static Task getTask(short id){
    	Task toGet = null;
    	for(int i = 0; i < index; i++){
    		if(tasks[i].executionContextID == id){
    			toGet = tasks[i];
    			break;
    		}
    	}
    	return toGet;    
    } 
    
    protected static void addTask(Task t){
    	tasks[index] = t;
    	index++;    
    }
   
}
