
import javax.virtualsense.network.Packet;

public class Message extends Packet {
	public short temp;    	// data example
	public short pres;
	public short hum;
	public short counter;	// message counter
	
	public Message(short sender, short counter){
		super(sender);
		
		this.counter = counter;
	}
}