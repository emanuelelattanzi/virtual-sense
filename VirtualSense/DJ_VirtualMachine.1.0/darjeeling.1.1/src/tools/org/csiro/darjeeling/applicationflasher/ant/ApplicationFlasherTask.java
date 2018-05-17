/*
 *  ApplicationFlasherTask.java
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
 * Applcation flasher task.
 * 
 * @author Emanuele Lattanzi
 *
 */
package org.csiro.darjeeling.applicationflasher.ant;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.*;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.*;

public class ApplicationFlasherTask extends Task
{
		private static final String PROGRAMMER = "mspdebug";
		private static final String OPTIONS = "-j";
		private static final String DRIVER = "tilib";
		
        private String flasherfile;
        private String ttyport;
        private String firmware;
        private String saddress;
        private String apps;
        private String running;
        private String appdir;
                
        public void execute()
        {
                // make sure properties are set
                if (flasherfile==null)
                        throw new BuildException("Property 'flasherfile' not set");
                if (ttyport==null)
                        throw new BuildException("Property 'ttyport' not set");
                if (firmware==null)
                        throw new BuildException("Property 'firmware' not set");
                if (apps==null)
                        throw new BuildException("Property 'apps' not set");
                if (running==null)
                    throw new BuildException("Property 'running' not set");
                if (saddress==null)
                        throw new BuildException("Property 'sddress' not set");
                if (appdir==null)
                    throw new BuildException("Property 'appdir' not set");

                // write loader C file
                try {
                        FileOutputStream fout = new FileOutputStream(flasherfile);
                        PrintWriter writer = new PrintWriter(fout);
                        writeTexasInstrumentsTeXTFile(writer);
                        //writeFlasherFile(writer );
                        writer.flush();
                        writer.close();
                        fout.close();
                } catch (IOException ioex) {
                        throw new org.apache.tools.ant.BuildException("IO error while writing: " + flasherfile);
                }                
        }
        
        private static String[] split(String str)
        {
                String trim;
                
                // trim the string
                trim = str.trim();

                if (trim.length()==0)
                        return new String[0];

                // split on comma, remove any whitespace before and after the comma
                return trim.split("\\s*,\\s*");
                                
        }
        
        private static boolean isInList(String[] list, String str)
        {
                for (String nat : list) if (nat.equals(str)) return true;
                return false;
        }
        
        public void writeTexasInstrumentsTeXTFile(PrintWriter out) throws IOException
        {
        		int address = Integer.parseInt(saddress, 16);
        		String flashCCA = "";
        		System.out.println("Starting address is "+saddress);
        		int startingAddresses[] = new int[split(apps).length];
        		String appsNames[] = new String[split(apps).length];
        		int i = 0;
        		String progApp = PROGRAMMER+" "+OPTIONS+" -d "+ttyport+" "+DRIVER+" ";
                
        		// convert the elf firmware file in TITex file
        		// using the sreg_cat linux commnad 
        		//srec_cat darjeeling.hex  -intel -Output darjeeling.txt -Texas_Instruments_TeXT
        		// the .hex firmware
        		String hexFirmware = firmware.substring(0, firmware.length()-3)+"hex";
        		Process p = Runtime.getRuntime().exec("srec_cat "
        				+hexFirmware+" -intel -Output - -Texas_Instruments_TeXT", null, new File("."));

                BufferedReader d = new BufferedReader(new InputStreamReader(p.getInputStream()));
                //p.waitFor();
                String tmp;
                while((tmp = d.readLine()) != null){
                	if(tmp.indexOf("@27FFD4")>=0)
                		flashCCA = d.readLine();
                	else if(!tmp.equals("q"))
                    	out.println(tmp);
                }
          	  	out.println("@"+Integer.toHexString(address));
          	  	int charsInLine = 0;
          	  	for (String app: split(apps)) 
                {
                	char array_string[] = {'_', '_','_','_','_','_','_','_','_','_'};
                	for(int j = 0; (j < array_string.length) && (j<app.length()); j++)
                		array_string[j] = app.charAt(j);
                	
                	//out.println("# this line will flash the application " + app);
                    log("Writing "+app+" application content",Project.MSG_INFO);
                    startingAddresses[i] = address;
                    appsNames[i] = new String(array_string);
                    i++;
                    try{
                    	  // Open the file that is the first 
                    	  // command line parameter
                    	  FileInputStream fstream = new FileInputStream(appdir+File.separator+app+"_di.hexd");
                    	  // Get the object of DataInputStream
                    	  DataInputStream in = new DataInputStream(fstream);
                    	  BufferedReader br = new BufferedReader(new InputStreamReader(in));
                    	  String strLine;
                    	  //Read File Line By Line
                    	  // read the first line containing the byte numbers
                    	  int total = Integer.parseInt(br.readLine());
                    	  while ((strLine = br.readLine()) != null)   {
                    		  for(int h = 0; h < strLine.length(); h++){
                    			  char tmpc = strLine.charAt(h);
                    			  if(tmpc == ' '){
                    				  charsInLine++;
                    			  }
                    			  if(charsInLine == 16){
                    				  charsInLine = 0;
                    				  out.println();
                    			  }else 
                    				  out.print(tmpc);
                    		  }
                    	  }
                    	  //out.println();
                    	  address+=total;
                    	  //Close the input stream
                    	  in.close();
                   	  }catch (Exception e){//Catch exception if any
                   	    	System.err.println("Error: " + e.getMessage());
                   	  }
                }
          	  	if(charsInLine != 16)
          	  		out.println();
                String table = "";
                for(int h = 0; h < startingAddresses.length; h++){
                	byte id_0 = (byte)((char)(h+1) & 0xff);                	
                	byte id_1 = (byte)(((char)(h+1)>> 8) & 0xff);
                	System.out.println("id_0: "+id_0+" id_1:"+id_1);
                	byte ad_0 = (byte)((startingAddresses[h]) & 0xff);
                	byte ad_1 = (byte)(((startingAddresses[h])>> 8) & 0xff);
                	byte ad_2 = (byte)(((startingAddresses[h])>> 16) & 0xff);
                	byte ad_3 = (byte)(((startingAddresses[h])>> 24) & 0xff);
                	System.out.println("ad_0: "+ad_0+" ad_1:"+ad_1+" ad_2:"+ad_2+" ad_3:"+ad_3);
                	byte s_bytes[] = appsNames[h].getBytes();
                	for(int f = 0; f < s_bytes.length; f++)
                		table+=""+purgeHex(Integer.toHexString(s_bytes[f]))+" ";
                	table+=""+purgeHex(Integer.toHexString(id_0))+" "+
                	purgeHex(Integer.toHexString(id_1))+" "+
                	purgeHex(Integer.toHexString(ad_0))+" "+
                	purgeHex(Integer.toHexString(ad_1))+" "+
                	purgeHex(Integer.toHexString(ad_2))+" "+
                	purgeHex(Integer.toHexString(ad_3))+" "+(running.indexOf(appsNames[h])>=0?"01 01 ":"00 00 "); //LOADED AND RUNNIG for now static
                }
                out.println("@27F7D4"); //table address
                charsInLine = 0;
                for(int h = 0; h < table.length(); h++){
      			  char tmpc = table.charAt(h);
      			  if(tmpc == ' '){
      				  charsInLine++;
      			  }
      			  if(charsInLine == 16){
      				  charsInLine = 0;
      				  out.println();
      			  }else 
      				  out.print(tmpc);
      		    }
                // WRITE the FLASH_CCA section for Cortex-M3
                out.println();
                out.println("@27FFD4");
                out.println(flashCCA);                
                out.println("q");
                log("Table writed: "+table,Project.MSG_INFO);
        }
        
        public void writeFlasherFile(PrintWriter out)
        {
        		int address = Integer.parseInt(saddress, 16);
        		System.out.println("Starting address is "+address);
        		int startingAddresses[] = new int[split(apps).length];
        		String appsNames[] = new String[split(apps).length];
        		int i = 0;
        		String progApp = PROGRAMMER+" "+OPTIONS+" -d "+ttyport+" "+DRIVER+" ";
                out.println("#!/bin/bash");
                // the line needed to program the drjeeling firmware
                out.println(progApp+" 'prog "+firmware+"'");
                
                
                for (String app: split(apps)) 
                {
                	char array_string[] = {'_', '_','_','_','_','_','_','_','_','_'};
                	for(int j = 0; (j < array_string.length) && (j<app.length()); j++)
                		array_string[j] = app.charAt(j);
                	
                	out.println("# this line will flash the application " + app);
                    log("Writing "+app+" application content",Project.MSG_INFO);
                    startingAddresses[i] = address;
                    appsNames[i] = new String(array_string);
                    i++;
                    try{
                    	  // Open the file that is the first 
                    	  // command line parameter
                    	  FileInputStream fstream = new FileInputStream(appdir+File.separator+app+"_di.hexd");
                    	  // Get the object of DataInputStream
                    	  DataInputStream in = new DataInputStream(fstream);
                    	  BufferedReader br = new BufferedReader(new InputStreamReader(in));
                    	  String strLine;
                    	  //Read File Line By Line
                    	  // read the first line containing the byte numbers
                    	  int total = Integer.parseInt(br.readLine());
                    	  while ((strLine = br.readLine()) != null)   {
                    	  // Print the content on the console
                    	  	out.println(progApp+" 'mw 0x"+Integer.toHexString(address)+" " +strLine+"'");
                    	  	if(total > 256){
                    	  		address+=256;
                    	  		total-=256;
                    	  	}else 
                    	  		address+=total;                        	  	
                    	  }
                    	  //Close the input stream
                    	  in.close();
                   	  }catch (Exception e){//Catch exception if any
                   	    	System.err.println("Error: " + e.getMessage());
                   	  }
                }
                out.println("# now write the application table [name][app_id][address][loaded][running]");
                String table = "";
                for(int h = 0; h < startingAddresses.length; h++){
                	byte id_0 = (byte)((char)(h+1) & 0xff);                	
                	byte id_1 = (byte)(((char)(h+1)>> 8) & 0xff);
                	System.out.println("id_0: "+id_0+" id_1:"+id_1);
                	byte ad_0 = (byte)((char)(startingAddresses[h]) & 0xff);
                	byte ad_1 = (byte)(((char)(startingAddresses[h])>> 8) & 0xff);
                	System.out.println("ad_0: "+ad_0+" ad_1:"+ad_1);
                	byte s_bytes[] = appsNames[h].getBytes();
                	for(int f = 0; f < s_bytes.length; f++)
                		table+=""+purgeHex(Integer.toHexString(s_bytes[f]))+" ";
                	table+=""+Integer.toHexString(id_0)+" "+
                	purgeHex(Integer.toHexString(id_1))+" "+
                	purgeHex(Integer.toHexString(ad_0))+" "+
                	purgeHex(Integer.toHexString(ad_1))+" "+(running.indexOf(appsNames[h])>=0?" x1 x1 ":" 00 00 "); //LOADED AND RUNNIG for now static
                }
                out.println(progApp+" 'mw 0x20000 "+table+"'");
                log("Table writed: "+table,Project.MSG_INFO);
        }
        
        public void setFlasherfile(String ffile)
        {
                this.flasherfile = ffile;
        }
        
        public void setTtyport(String ttport)
        {
                this.ttyport = ttport;
        }
        
        public void setFirmware(String fware)
        {
                this.firmware = fware;
        }
        
        public void setSaddress(String sad)
        {
                this.saddress = sad;
        }    
        public void setApps(String ap)
        {
                this.apps = ap;
        } 
        public void setRunning(String ru)
        {
                this.running = ru;
        } 
        public void setAppdir(String ap)
        {
                this.appdir = ap;
        } 
        public String purgeHex(String toPurge){
        	String ret = toPurge;
        	log("Request to Purge "+toPurge,Project.MSG_INFO);
        	if (ret.length() > 2){
        		ret = ret.substring(ret.length()-2);
        		log("Purging "+toPurge+" into "+ret,Project.MSG_INFO);
        	}
        	if (ret.length() == 1)
        		ret = "0"+ret;
        	return ret;
        }
}
