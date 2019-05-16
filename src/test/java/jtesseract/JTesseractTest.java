package jtesseract;

import jtesseract.JTesseract;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;
//import org.junit.Test;

public class JTesseractTest extends TestCase
{
	/**
     * Create the test case
     *
     * @param testName name of the test case
     */
	public JTesseractTest(String testName)
	{
		super(testName);
	}

	/**
     * @return the suite of tests being tested
     */
	public static Test suite()
	{
		return new TestSuite(JTesseract.class);
	}

	/**
	 * Test creation of a a new JTesseract object
	 */
	@Test
	public void testCreate()
	{
		JTesseract tess = new JTesseract();
		assertTrue(tess != null);
	}
}
