package jtesseract;

/**
 * A simple mutable wrapper class for integers. 
 * 
 * @author junior
 *
 */
public class JTessInt {
	
	private int value;
	
	public JTessInt(int value)
	{
		this.value = value;
	}
	
	public JTessInt()
	{
		
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}
	
}
