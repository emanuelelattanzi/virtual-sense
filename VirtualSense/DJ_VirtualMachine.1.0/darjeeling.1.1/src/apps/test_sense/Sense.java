/*
 *	Blink.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
import javax.virtualsense.actuators.Leds;

import javax.virtualsense.digitalio.bus.I2C;

import javax.virtualsense.sensors.*;
import javax.virtualsense.VirtualSense;
import javax.darjeeling.Darjeeling;
import javax.virtualsense.powermanagement.PowerManager;

public class Sense
{
	public static byte SI7020_ADDRESS = (byte)0x80;
	public static byte SI7020_READ_TEMP_NOHOLD = (byte)0xF3;
	public static byte SI7020_READ_HUMID_NOHOLD = (byte)0xF5;
	public static byte SI7020_READ_USER_REG_1 = (byte)0xE7;
	public static byte SI7020_WRITE_USER_REG_1 = (byte)0xE6;
	public static byte SI7020_RESET = (byte)0xFE;
	
    public static void motemain()
    {
   	 	I2C.enable();
   	 	Thread.sleep(1000);
   	 	System.out.println("_Temp(mC)_Pressure(hPa)_Humidity(%mRH)");
   	 	short temp = (short)0;
   	 	short pres = (short)0;
   	 	short hum = (short)0;
   	 	Leds.setLed(Leds.LED0, true);
   	 	Leds.setLed(Leds.LED1, false);
   	 	Leds.setLed(Leds.LED2, false);
   	 	//initSI7020();
    	
        while(true)
        {
        	Leds.setLed(Leds.LED1, true);
        	pres = Pressure.getValue();
        	temp = readTempSI7020();
        	hum = readHumSI7020();
        	
        	System.out.print("_");
        	System.out.print(temp);System.out.print("_");
        	System.out.print(pres);System.out.print("_");
        	System.out.println(hum);
        	Leds.setLed(Leds.LED1, false);
        	
        	Thread.sleep(5000);//Thread.sleep(75500);
        	//PowerManager.scheduleRTCInterruptAfter(1);
        	//PowerManager.standby();
        	
        	/*System.out.print("ADC0: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL0, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);

        	System.out.print("ADC1: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL1, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);
        	
        	System.out.print("ADC2: ");
        	System.out.print(ADC.readIntRef(ADC.CHANNEL2, ADC.REF2_5V));
        	System.out.println("mV");
        	Thread.sleep(500);
        	
        	System.out.print("Battery voltage: ");
        	System.out.print(ADC.readVccLevel());
        	System.out.println("mV");
        	Thread.sleep(500);  	
        	
        	System.out.print("Temperature: ");
        	System.out.print(Temperature.getValue());
        	System.out.println("mC");
        	Thread.sleep(500);*/
        	
        	/*System.out.print("Board temperature: ");
        	System.out.print(Temperature.getBoardValue());
        	System.out.println("mC");
        	Thread.sleep(500);
        	
        	System.out.print("Pressure: ");
        	System.out.print(Pressure.getValue());
        	System.out.println("hPa");
        	Thread.sleep(500);*/
        	
        	/*System.out.print("Light intensity: ");
        	System.out.print(Light.getValue());
        	System.out.println("lx");
        	
            System.out.print("Free mem: ");
            System.out.println(Runtime.freeMemory());
            System.out.println("");
            Thread.sleep(1000);
        	
        	System.out.print("Temp SI7020: ");
	    	System.out.print(readTempSI7020());
	    	System.out.println("mC");
	    	
	    	System.out.print("Humid SI7020: ");
	    	System.out.print(readHumSI7020());
	    	System.out.println("%mRH");
	    	Thread.sleep(1000);*/
        }
    }
    
    /*public static void initSI7020() {
    	
    	I2C.start();
   	 	I2C.write(SI7020_ADDRESS);
   	 	I2C.write(SI7020_READ_USER_REG_1);
	   	 
   	 	I2C.start();
	   	I2C.write((byte)(SI7020_ADDRESS | 1));
	
	   	byte user_reg = I2C.read(false);
	   	 //byte b2 = I2C.read(false);
	   	 
	   	 
	   	 //System.out.print(" ");
	   	 //System.out.println(b2);
	   	I2C.stop();
    	
	   	System.out.print("Read user reg: ");
	   	System.out.println(user_reg);
	   	
	   	
	   	
	   	
	   	I2C.start();
   	 	I2C.write(SI7020_ADDRESS);
   	 	I2C.write(SI7020_WRITE_USER_REG_1);
   	 	
   	 	System.out.print("Write user ack: ");
   	 	System.out.println((I2C.write((byte)0x00)?"True":"False"));
	   	I2C.stop();
    	
	   	System.out.print("Read user reg: ");
	   	System.out.println(user_reg);
	   	
	   	
	   	
	   	
	   	
	   	I2C.start();
   	 	I2C.write(SI7020_ADDRESS);
   	 	I2C.write(SI7020_READ_USER_REG_1);
	   	 
   	 	I2C.start();
	   	I2C.write((byte)(SI7020_ADDRESS | 1));
	
	   	user_reg = I2C.read(false);
	   	 //byte b2 = I2C.read(false);
	   	 
	   	 
	   	 //System.out.print(" ");
	   	 //System.out.println(b2);
	   	I2C.stop();
    	
	   	System.out.print("Read user reg after write: ");
	   	System.out.println(user_reg);
	   	
	   	
	   	user_reg = (byte)0x0A;
	   	System.out.print("Read user reg : ");
	   	System.out.println(user_reg);
	   	
    }*/
    
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
