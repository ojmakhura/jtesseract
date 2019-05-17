package jtesseract;

/**
 * A simple mutable wrapper class for booleans. 
 * 
 * @author junior
 *
 */
public class JTessBoolean {
	
	private int value;
	
	public JTessBoolean(int value)
	{
		this.value = value;
	}
	
	public JTessBoolean()
	{
		
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}
}
