
package javax.virtualsense.network.protocols.minpath;

import javax.virtualsense.network.UnicastPacket;

public class DataMsg extends UnicastPacket {
	public short sender_id; 
	public short counter; 
	public short route;
	//public char label[];
	//public short value;
	
	public DataMsg(short sender, short counter)
	{
		this.sender_id = sender;
		this.counter = counter;
		this.route = (short)0;
		//this.label = new char[5];
		
		/*this.noise = (short)0;
		this.co2 = (short)0;
		this.in = (short)0;
		this.out = (short)0;
		this.temp = (short)0;
		this.pressure = (short)0;
		this.light = (short)0;*/
	}
	
}