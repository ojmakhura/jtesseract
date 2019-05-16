package jtesseract;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;

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
	 * 
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
