package jtesseract;

/**
 * A simple mutable wrapper class for booleans. 
 * 
 * @author junior
 *
 */
public class JTessDouble {
	
	private int value;
	
	public JTessDouble(int value)
	{
		this.value = value;
	}
	
	public JTessDouble()
	{
		
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}
}
