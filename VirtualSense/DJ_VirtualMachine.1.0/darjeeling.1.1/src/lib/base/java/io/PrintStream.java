/**
 * 
 */
package java.io;

/**
 * @author Michael Maaser
 *
 */
public class PrintStream extends OutputStream {

	public PrintStream(){
		
	}
	
	public void println(String string) {
		OutputStream.write(string);
		OutputStream.write("\n");
	}
	
	public void println(int i) {
		OutputStream.write(i);
		OutputStream.write("\n");
	}
	public void println(short i) {
		OutputStream.write(i);
		OutputStream.write("\n");
	}
	
	public void print(String string) {
		OutputStream.write(string);		
	}
	
	public void print(short i) {
		OutputStream.write(i);		
	}
	public void print(int i) {
		OutputStream.write(i);		
	}

}
