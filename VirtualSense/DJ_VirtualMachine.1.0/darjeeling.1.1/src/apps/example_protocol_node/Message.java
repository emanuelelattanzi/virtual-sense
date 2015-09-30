
import javax.virtualsense.network.protocols.minpath.DataMsg;

public class Message extends DataMsg {
	public short value;    // data example
	
	public Message(short sender, short counter, short value){
		super(sender, counter);
		
		this.value = value;
	}
}