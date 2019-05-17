/*
 * JTesseract.java
 *
 * Copyright 2019 Onalenna Junior Makhura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package jtesseract;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;
/**
 * This is the main wrapper class for tesseract OCR library.
 * 
 */
public class JTesseract
{
	static{
		System.loadLibrary("jtesseract");
	}
	
	/**
	 * 
	 * 
	 */
	public native void initialise(String datapath, String lang, int engine);
	
	/**
	 * 
	 * 
	 */
	public native void setPageSegMode(int segMode);
	
	/**
	 * 
	 * 
	 */
	public native void setImage(String filepath); 
	
	/**
	 * 
	 * 
	 */
	public native String getUTF8Text(); 
	
	/**
	 * 
	 * 
	 */
	public native void clean(); 

	/**
	 * Create a new JTesseract object. It is a utility method for the constructor
	 * 
	 */
	public native void create(); 

	/**
	 * 
	 */
	public static native String Version();

	/**
	 * Set the tesseract image from a byte array, number of rows, columns and channels.
	 * The data should be from an RGB image where each channel is 1 byte per pixel.
	 */
	public native void setFromBufferedImage(byte[] data, int rows, int cols, int channels);

	/**
	 * Writes the thresholded image to stderr as a PBM file on receipt of a
	 * SIGSEGV, SIGFPE, or SIGBUS signal. (Linux/Unix only).
	 */
	public static native void catchSignals();

	/**
	 * Set the name of the input file. Needed for training and
	 * reading a UNLV zone file, and for searchable PDF output.
	 */
	public native void setInputName(String name);

	/**
	 * These functions are required for searchable PDF output.
	 * We need our hands on the input file so that we can include
	 * it in the PDF without transcoding. If that is not possible,
	 * we need the original image. Finally, resolution metadata
	 * is stored in the PDF so we need that as well.
	 */
	public native String getInputName();
	
	// Takes ownership of the input pix.
	//void SetInputImage(Pix *pix);
	//Pix* GetInputImage();
	public native int getSourceYResolution();
	public native String getDatapath();

	/** Set the name of the bonus output files. Needed only for debugging. */
	public native void setOutputName(String name);

	/**
	 * Set the value of an internal "parameter."
	 * Supply the name of the parameter and the value as a string, just as
	 * you would in a config file.
	 * Returns false if the name lookup failed.
	 * Eg SetVariable("tessedit_char_blacklist", "xyz"); to ignore x, y and z.
	 * Or SetVariable("classify_bln_numeric_mode", "1"); to set numeric-only mode.
	 * SetVariable may be used before Init, but settings will revert to
	 * defaults on End().
	 *
	 * Note: Must be called after Init(). Only works for non-init variables
	 * (init variables should be passed to Init()).
	 */
	public native boolean setVariable(String name, String value);
	public native boolean setDebugVariable(String name, String value);

	/**
	 * Returns true if the parameter was found among Tesseract parameters.
	 * Fills in value with the value of the parameter.
	 */
	public native boolean getIntVariable(String name, JTessInt value);
	public native boolean getBoolVariable(String name, JTessBoolean value);
	public native boolean getDoubleVariable(String name, JTessDouble value);

	/**
	 * Returns the pointer to the string that represents the value of the
	 * parameter if it was found among Tesseract parameters.
	 */
	public native String getStringVariable(String name);

	/**
	 * Print Tesseract parameters to the given file.
	 */
	public native void printVariables(String filename);

	/**
	 * Get value of named variable as a string, if it exists.
	 */
	public native boolean getVariableAsString(String name, String val);


	/**
	 * 
	 * 
	 */ 
	public JTesseract()
	{
		create();
	}

	/**
	 * Get byte array from the BufferedImage object. The image is first converted into a 3 channel
	 * BGR image with BufferedImage.TYPE_3BYTE_BGR for compartibility with OpenCV Mat object.
	 * 
	 * @param image
	 * @return
	 */
	public byte[] getImageByteData(BufferedImage image)
	{
		BufferedImage convertedImg = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_3BYTE_BGR);
		convertedImg.getGraphics().drawImage(image, 0, 0, null);		
		WritableRaster raster = convertedImg.getRaster();
		DataBufferByte buf = (DataBufferByte) raster.getDataBuffer();
		
		return buf.getData();
	}
}
