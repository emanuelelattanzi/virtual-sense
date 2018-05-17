
import javax.virtualsense.network.Packet;

public class Message extends Packet {
	public short value;    	// data example
	public short counter;	// message counter
	private int a1;
	private int a2;
	private int a3;
	private int a4;
	private int a5;
	private int a6;
	private int a7;
	private int a8;
	private int a9;
	private int a10;
	private int a11;
	private int a12;
	private int a13;
	private int a14;
	private int a15;
	private int a16;
	private int a17;
	private int a18;
	private int a19;
	private int a20;
	
	public Message(short sender, short counter, short value){
		super(sender);
		
		this.counter = counter;
		this.value = value;		
		
	}
}