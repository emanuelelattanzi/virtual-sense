
import javax.virtualsense.network.Packet;

public class Message extends Packet {
	public short value;    	// data example
	public short counter;	// message counter
	public short a1;	
	public short a2;	
	public short a3;	
	public short a4;	
	public short a5;	// 30 bytes
	public short a6;	
	public short a7;	
	public short a8;	
	public short a9;	
	public short a10;	// 40 bytes
	public short a11;	  // FOR BROADCAST PACKET CONSIDER TWO MORE BYTES
	public short a12;	
	public short a13;	
	public short a14;	
	public short a15;	// 50 bytes
	public short a16;	
	public short a17;	
	public short a18;	
	public short a19;	
	public short a20;	// 60 bytes
	public short a21;	
	public short a22;	
	public short a23;	
	public short a24;	
	public short a25;	// 70 bytes
	public short a26;	
	public short a27;	
	public short a28;	
	public short a29;	
	public short a30;	// 80 byes
	public short a31;	
	public short a32;	
	public short a33;	
	public short a34;	
	public short a35;	// 90 bytes 
	public short a36;	
	/*public short a37;	
	public short a38;	
	public short a39;	
	public short a40;	// 100 bytes
	public short a41;	
	/*public short a42;	
	public short a43;	
	public short a44;	
	public short a45;	// 110 bytes
	public short a46;	
	public short a47;	
	public short a48;
	public short a49;
	//public short a50;*/
	//public short a51;
		
	
	
	public Message(short sender, short counter, short value){
		super(sender);
		
		this.counter = counter;
		this.value = value;
		this.a1   = counter;
		this.a2   = 2;
		this.a3   = 3;
		this.a4   = 4;
		this.a5   = 5;
		this.a6   = 6;
		this.a7   = 7;
		this.a8   = 8;
		this.a9   = 9;
		this.a10  = counter;
		this.a11  = 11; // FOR BROADCAST PACKET CONSIDER TWO MORE BYTES
		this.a12  = 12;
		this.a13  = 13;
		this.a14  = 14;
		this.a15  = 15;
		this.a16  = 16;
		this.a17  = 17;
		this.a18  = 18;
		this.a19  = 19;
		this.a20  = counter;
		this.a21   = 1;
		this.a22   = 2;
		this.a23   = 3;
		this.a24   = 4;
		this.a25   = 5;  // 70 bytes
		this.a26   = 6;
		this.a27   = 7;
		this.a28   = 8;
		this.a29   = 9;
		this.a30  = 10;   // 80 bytes
		this.a31  = 11;
		this.a32  = 12;
		this.a33  = 13;
		this.a34  = 14;
		this.a35  = 15;   // 90 bytes
		this.a36  = 16;
		/*this.a37  = 17;
		this.a38  = 18;
		this.a39  = 19;
		this.a40  = counter;   // 100 byte
		this.a41   = 1;
		/*this.a42   = 2;
		this.a43   = 3;
		this.a44   = 4;
		this.a45   = 5;       // 110 byes
		this.a46   = 6;
		this.a47   = 7;
		this.a48   = 8;
		this.a49   = 9;
		//this.a50   = 10;*/
		//this.a51   = 11;
		
		
		
	}
}