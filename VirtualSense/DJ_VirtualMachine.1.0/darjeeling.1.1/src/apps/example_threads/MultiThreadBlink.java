
import javax.virtualsense.actuators.Leds;
import javax.virtualsense.powermanagement.PowerManager;

public class MultiThreadBlink
{
	public static void motemain()
    {
		Buffer b = new Buffer();
    	Th thread1 = new Th(b, 1000, (short)1);
    	Th thread2 = new Th(b, 500, (short)2);
    	
    	thread1.start();
    	thread2.start();
    
    	   	
    	while(true){
    		Leds.setLed(Leds.LED1,false);
    		Leds.setLed(Leds.LED2,false);
    		b.temp-=4;
    		Thread.sleep(2000);
    		System.out.print("tmp: ");
    		System.out.println(b.temp);
    		
            
    	}
    }
}
