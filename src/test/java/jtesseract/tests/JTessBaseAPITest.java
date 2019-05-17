package jtesseract;

import jtesseract.JTesseract;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

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
		return new TestSuite(JTesseractTest.class);
	}

	/**
	 * Test creation of a a new JTesseract object
	 */
	public void testCreate()
	{
		//JTesseract tess = new JTesseract();
		assertTrue(true);
	}
}
