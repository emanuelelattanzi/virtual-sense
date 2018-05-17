
import javax.virtualsense.network.Packet;

public class Message extends Packet {
	public short value;    	// data example
	public short counter;	// message counter
	public short a1;	
	public short a2;	
	public short a3;	
	public short a4;	
	public short a5;	
	public short a6;	
	public short a7;	
	public short a8;	
	public short a9;	
	public short a10;	
	public short a11;	
	public short a12;	
	public short a13;	
	public short a14;	
	public short a15;	
	public short a16;	
	public short a17;	
	public short a18;	
	public short a19;	
	public short a20;	
	
	public Message(short sender, short counter, short value){
		super(sender);
		
		this.counter = counter;
		this.value = value;
		this.a1   = 1;
		this.a2   = 2;
		this.a3   = 3;
		this.a4   = 4;
		this.a5   = 5;
		this.a6   = 6;
		this.a7   = 7;
		this.a8   = 8;
		this.a9   = 9;
		this.a10  = 10;
		this.a11  = 11;
		this.a12  = 12;
		this.a13  = 13;
		this.a14  = 14;
		this.a15  = 15;
		this.a16  = 16;
		this.a17  = 17;
		this.a18  = 18;
		this.a19  = 19;
		this.a20  = counter;
		
	}
}