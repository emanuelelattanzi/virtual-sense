
import javax.virtualsense.network.Packet;

public class Message extends Packet {
	public short value;    	// data example
	public short counter;	// message counter
	
	public Message(short sender, short counter, short value){
		super(sender);
		
		this.counter = counter;
		this.value = value;
	}
}