
import javax.virtualsense.network.BroadcastPacket;

public class Message extends BroadcastPacket {
	public short LED0;    // data example
	public short LED1;    // data example
	public short LED2;    // data example
	
	public Message(short led0, short led1, short led2){
		
		this.LED0 = led0;
		this.LED1 = led1;
		this.LED2 = led2;
	}
}