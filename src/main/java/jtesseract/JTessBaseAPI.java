package jtesseract;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;
import java.util.ArrayList;

public class JTessBaseAPI {
	static {
		System.loadLibrary("jtesseract");
	}

	/**
	 * 
	 * @param datapath
	 * @param lang
	 * @param engine
	 */
	public native void initialise(String datapath, String lang, int engine);

	/**
	 * 
	 * @param segMode
	 */
	public native void setPageSegMode(int segMode);

	/**
	 * 
	 * @param filepath
	 */
	public native void setImage(String filepath);

	/**
	 * 
	 * @return
	 */
	public native String getUTF8Text();

	/**
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
	 * Set the tesseract image from a byte array, number of rows, columns and
	 * channels. The data should be from an RGB image where each channel is 1 byte
	 * per pixel.
	 *
	 * @param data
	 * @param rows
	 * @param cols
	 * @param channels
	 */
	public native void setFromBufferedImage(byte[] data, int rows, int cols, int channels);

	/**
	 * Writes the thresholded image to stderr as a PBM file on receipt of a SIGSEGV,
	 * SIGFPE, or SIGBUS signal. (Linux/Unix only).
	 */
	public static native void catchSignals();

	/**
	 * Set the name of the input file. Needed for training and reading a UNLV zone
	 * file, and for searchable PDF output.
	 */
	public native void setInputName(String name);

	/**
	 * These functions are required for searchable PDF output. We need our hands on
	 * the input file so that we can include it in the PDF without transcoding. If
	 * that is not possible, we need the original image. Finally, resolution
	 * metadata is stored in the PDF so we need that as well.
	 * 
	 * @return
	 */
	public native String getInputName();

	/**
	 * Takes ownership of the input pix.
	 * void SetInputImage(Pix *pix);
	 * Pix* GetInputImage(); 
	 * 
	 * @return
	 */
	public native int getSourceYResolution();
	
	/**
	 * 
	 * @return
	 */
	public native String getDatapath();

	/** Set the name of the bonus output files. Needed only for debugging. 
	 * 
	 * @param name
	 */
	public native void setOutputName(String name);

	/**
	 * Set the value of an internal "parameter." Supply the name of the parameter
	 * and the value as a string, just as you would in a config file. Returns false
	 * if the name lookup failed. Eg SetVariable("tessedit_char_blacklist", "xyz");
	 * to ignore x, y and z. Or SetVariable("classify_bln_numeric_mode", "1"); to
	 * set numeric-only mode. SetVariable may be used before Init, but settings will
	 * revert to defaults on End().
	 *
	 * Note: Must be called after Init(). Only works for non-init variables (init
	 * variables should be passed to Init()).
	 * 
	 * @param name
	 * @param value
	 * @return
	 */
	public native boolean setVariable(String name, String value);

	public native boolean setDebugVariable(String name, String value);

	/**
	 * Returns true if the parameter was found among Tesseract parameters. Fills in
	 * value with the value of the parameter. 
	 * 
	 * @param name
	 * @param value
	 * @return
	 */
	public native boolean getIntVariable(String name, JTessInt value);
	public native boolean getBoolVariable(String name, JTessBoolean value);
	public native boolean getDoubleVariable(String name, JTessDouble value);

	/**
	 * Returns the pointer to the string that represents the value of the parameter
	 * if it was found among Tesseract parameters.
	 * 
	 * @param name
	 * @return
	 */
	public native String getStringVariable(String name);

	/**
	 * Print Tesseract parameters to the given file.
	 * 
	 * @param filename
	 */
	public native void printVariables(String filename);

	/**
	 * Get value of named variable as a string, if it exists.
	 * 
	 * @param name
	 * @param val
	 * @return
	 */
	public native boolean getVariableAsString(String name, String val);

	/**
	 * Returns the languages string used in the last valid initialization. If the
	 * last initialization specified "deu+hin" then that will be returned. If hin
	 * loaded eng automatically as well, then that will not be included in this
	 * list. To find the languages actually loaded use GetLoadedLanguagesAsVector.
	 * The returned string should NOT be deleted.
	 *
	 * @return
	 */
	public native String getInitLanguagesAsString();

	/**
	 * Returns the loaded languages in the vector of STRINGs. Includes all languages
	 * loaded by the last Init, including those loaded as dependencies of other
	 * loaded languages.
	 *
	 * @return
	 */
	public native ArrayList<String> getLoadedLanguagesAsVector();

	/**
	 * Returns the available languages in the sorted vector of STRINGs.
	 *
	 * @return
	 */
	public native ArrayList<String> getAvailableLanguagesAsVector();

	/**
	 * Init only the lang model component of Tesseract. The only functions
	 * that work after this init are SetVariable and IsValidWord.
	 * WARNING: temporary! This function will be removed from here and placed
	 * in a separate API at some future time.
	 *
	 * @param datapath
	 * @param language
	 * @return
	 */
	public native int initLangMod(String datapath, String language);

	/**
	 * Init only for page layout analysis. Use only for calls to SetImage and
	 * AnalysePage. Calls that attempt recognition will generate an error.
	 */
	public native void initForAnalysePage();

	/**
	 * Read a "config" file containing a set of param, value pairs.
	 * Searches the standard places: tessdata/configs, tessdata/tessconfigs
	 * and also accepts a relative or absolute path name.
	 * Note: only non-init params will be set (init params are set by Init()).
	 *
	 * @param filename
	 */
	public native void readConfigFile(String filename);

	/** 
	 * Same as above, but only set debug params from the given config file. 
	 * 
	 */
	public native void readDebugConfigFile(String filename);


	/** 
	 * Return the current page segmentation mode.
	 * 
	 * @return
	 */
	public native int getPageSegMode();

	/**
	 * Recognize a rectangle from an image and return the result as a string.
	 * May be called many times for a single Init.
	 * Currently has no error checking.
	 * Greyscale of 8 and color of 24 or 32 bits per pixel may be given.
	 * Palette color images will not work properly and must be converted to
	 * 24 bit.
	 * Binary images of 1 bit per pixel may also be given but they must be
	 * byte packed with the MSB of the first byte being the first pixel, and a
	 * 1 represents WHITE. For binary images set bytes_per_pixel=0.
	 * The recognized text is returned as a char* which is coded
	 * as UTF8 and must be freed with the delete [] operator.
	 *
	 * Note that TesseractRect is the simplified convenience interface.
	 * For advanced uses, use SetImage, (optionally) SetRectangle, Recognize,
	 * and one or more of the Get*Text functions below.
	 *
	 * @param imagedata
	 * @param bytes_per_pixel
	 * @param bytes_per_line
	 * @param left
	 * @param top
	 * @param width
	 * @param height
	 * @return
	 */
	public native String tesseractRect(byte[] imagedata,
						int bytes_per_pixel, int bytes_per_line,
						int left, int top, int width, int height);

	/**
	 * Call between pages or documents etc to free up memory and forget
	 * adaptive data.
	 */
	public native void clearAdaptiveClassifier();

	/**
	 * Set the resolution of the source image in pixels per inch so font size
	 * information can be calculated in results.  Call this after SetImage().
	 *
	 * @param ppi
	 */
	public native void setSourceResolution(int ppi);

	/**
	 * Restrict recognition to a sub-rectangle of the image. Call after SetImage.
	 * Each SetRectangle clears the recogntion results so multiple rectangles
	 * can be recognized with the same image.
	 *
	 * @param left
	 * @param top
	 * @param width
	 * @param height
	 */
	public native void setRectangle(int left, int top, int width, int height);

	/**
	 * In extreme cases only, usually with a subclass of Thresholder, it
	 * is possible to provide a different Thresholder. The Thresholder may
	 * be preloaded with an image, settings etc, or they may be set after.
	 * Note that Tesseract takes ownership of the Thresholder and will
	 * delete it when it it is replaced or the API is destructed.
	 */
	//public native void setThresholder(ImageThresholder* thresholder);

	/**
	 * Get a copy of the internal thresholded image from Tesseract.
	 * Caller takes ownership of the Pix and must pixDestroy it.
	 * May be called any time after SetImage, or after TesseractRect.
	 */
	//public native Pix* getThresholdedImage();

	/**
	 * Get the result of page layout analysis as a leptonica-style
	 * Boxa, Pixa pair, in reading order.
	 * Can be called before or after Recognize.
	 */
	//public native Boxa* GetRegions(Pixa** pixa);

	/**
	 * Get the textlines as a leptonica-style
	 * Boxa, Pixa pair, in reading order.
	 * Can be called before or after Recognize.
	 * If raw_image is true, then extract from the original image instead of the
	 * thresholded image and pad by raw_padding pixels.
	 * If blockids is not nullptr, the block-id of each line is also returned as an
	 * array of one element per line. delete [] after use.
	 * If paraids is not nullptr, the paragraph-id of each line within its block is
	 * also returned as an array of one element per line. delete [] after use.
	 */
	//public native Boxa* GetTextlines(const bool raw_image, const int raw_padding, Pixa** pixa, int** blockids, int** paraids);
	
	/*
		Helper method to extract from the thresholded image. (most common usage)
	*/
	/*Boxa* GetTextlines(Pixa** pixa, int** blockids) {
		return GetTextlines(false, 0, pixa, blockids, nullptr);
	}*/

	/**
	 * Get textlines and strips of image regions as a leptonica-style Boxa, Pixa
	 * pair, in reading order. Enables downstream handling of non-rectangular
	 * regions.
	 * Can be called before or after Recognize.
	 * If blockids is not nullptr, the block-id of each line is also returned as an
	 * array of one element per line. delete [] after use.
	 */
	//public native Boxa* GetStrips(Pixa** pixa, int** blockids);

	/**
	 * Get the words as a leptonica-style
	 * Boxa, Pixa pair, in reading order.
	 * Can be called before or after Recognize.
	 */
	//public native Boxa* GetWords(Pixa** pixa);

	/**
	 * Gets the individual connected (text) components (created
	 * after pages segmentation step, but before recognition)
	 * as a leptonica-style Boxa, Pixa pair, in reading order.
	 * Can be called before or after Recognize.
	 * Note: the caller is responsible for calling boxaDestroy()
	 * on the returned Boxa array and pixaDestroy() on cc array.
	 */
	//public native Boxa* GetConnectedComponents(Pixa** cc);

	/**
	 * Get the given level kind of components (block, textline, word etc.) as a
	 * leptonica-style Boxa, Pixa pair, in reading order.
	 * Can be called before or after Recognize.
	 * If blockids is not nullptr, the block-id of each component is also returned
	 * as an array of one element per component. delete [] after use.
	 * If blockids is not nullptr, the paragraph-id of each component with its block
	 * is also returned as an array of one element per component. delete [] after
	 * use.
	 * If raw_image is true, then portions of the original image are extracted
	 * instead of the thresholded image and padded with raw_padding.
	 * If text_only is true, then only text components are returned.
	 */
	/*public native Boxa* GetComponentImages(const PageIteratorLevel level,
							const bool text_only, const bool raw_image,
							const int raw_padding,
							Pixa** pixa, int** blockids, int** paraids);
	*/	
	// Helper function to get binary images with no padding (most common usage).
	/*Boxa* GetComponentImages(const PageIteratorLevel level,
							const bool text_only,
							Pixa** pixa, int** blockids) {
		return GetComponentImages(level, text_only, false, 0, pixa, blockids, nullptr);
	}*/

	/**
	 * Returns the scale factor of the thresholded image that would be returned by
	 * GetThresholdedImage() and the various GetX() methods that call
	 * GetComponentImages().
	 * Returns 0 if no thresholder has been set.
	 *
	 * @return
	 */
	public native int getThresholdedImageScaleFactor();

	/**
	 * Runs page layout analysis in the mode set by SetPageSegMode.
	 * May optionally be called prior to Recognize to get access to just
	 * the page layout results. Returns an iterator to the results.
	 * If merge_similar_words is true, words are combined where suitable for use
	 * with a line recognizer. Use if you want to use AnalyseLayout to find the
	 * textlines, and then want to process textline fragments with an external
	 * line recognizer.
	 * Returns nullptr on error or an empty page.
	 * The returned iterator must be deleted after use.
	 * WARNING! This class points to data held within the TessBaseAPI class, and
	 * therefore can only be used while the TessBaseAPI class still exists and
	 * has not been subjected to a call of Init, SetImage, Recognize, Clear, End
	 * DetectOS, or anything else that changes the internal PAGE_RES.
	 */
	//public native PageIterator* AnalyseLayout();
	//public native PageIterator* AnalyseLayout(bool merge_similar_words);

	/**
	 * Recognize the image from SetAndThresholdImage, generating Tesseract
	 * internal structures. Returns 0 on success.
	 * Optional. The Get*Text functions below will call Recognize if needed.
	 * After Recognize, the output is kept internally until the next SetImage.
	 */
	//public native int Recognize(ETEXT_DESC* monitor);

	/**
	 * Turns images into symbolic text.
	 *
	 * filename can point to a single image, a multi-page TIFF,
	 * or a plain text list of image filenames.
	 *
	 * retry_config is useful for debugging. If not nullptr, you can fall
	 * back to an alternate configuration if a page fails for some
	 * reason.
	 *
	 * timeout_millisec terminates processing if any single page
	 * takes too long. Set to 0 for unlimited time.
	 *
	 * renderer is responible for creating the output. For example,
	 * use the TessTextRenderer if you want plaintext output, or
	 * the TessPDFRender to produce searchable PDF.
	 *
	 * If tessedit_page_number is non-negative, will only process that
	 * single page. Works for multi-page tiff file, or filelist.
	 *
	 * Returns true if successful, false on error.
	 */
	//public native boolean processPages(const char* filename, const char* retry_config, int timeout_millisec, TessResultRenderer* renderer);

	// Does the real work of ProcessPages.
	//public native boolean ProcessPagesInternal(const char* filename, const char* retry_config, int timeout_millisec, TessResultRenderer* renderer);

	/**
	 * Turn a single image into symbolic text.
	 *
	 * The pix is the image processed. filename and page_index are
	 * metadata used by side-effect processes, such as reading a box
	 * file or formatting as hOCR.
	 *
	 * See ProcessPages for desciptions of other parameters.
	 */
	//public native boolean processPage(Pix* pix, int page_index, const char* filename,
	//				const char* retry_config, int timeout_millisec,
	//				TessResultRenderer* renderer);

	/**
	 * Get a reading-order iterator to the results of LayoutAnalysis and/or
	 * Recognize. The returned iterator must be deleted after use.
	 * WARNING! This class points to data held within the TessBaseAPI class, and
	 * therefore can only be used while the TessBaseAPI class still exists and
	 * has not been subjected to a call of Init, SetImage, Recognize, Clear, End
	 * DetectOS, or anything else that changes the internal PAGE_RES.
	 */
	//public native ResultIterator* GetIterator();

	/**
	 * Get a mutable iterator to the results of LayoutAnalysis and/or Recognize.
	 * The returned iterator must be deleted after use.
	 * WARNING! This class points to data held within the TessBaseAPI class, and
	 * therefore can only be used while the TessBaseAPI class still exists and
	 * has not been subjected to a call of Init, SetImage, Recognize, Clear, End
	 * DetectOS, or anything else that changes the internal PAGE_RES.
	 */
	//public native MutableIterator* GetMutableIterator();

	/**
	 * Make a HTML-formatted string with hOCR markup from the internal
	 * data structures.
	 * page_number is 0-based but will appear in the output as 1-based.
	 * monitor can be used to
	 *  cancel the recognition
	 *  receive progress callbacks
	 * Returned string must be freed with the delete [] operator.
	 */
	//public native char* GetHOCRText(ETEXT_DESC* monitor, int page_number);

	/**
	 * Make a HTML-formatted string with hOCR markup from the internal
	 * data structures.
	 * page_number is 0-based but will appear in the output as 1-based.
	 * Returned string must be freed with the delete [] operator.
	 */
	//public native char* GetHOCRText(int page_number);

	/**
	 * Make a TSV-formatted string from the internal data structures.
	 * page_number is 0-based but will appear in the output as 1-based.
	 * Returned string must be freed with the delete [] operator.
	 * 
	 * @param page_number
	 * @return
	 */
	public native String getTSVText(int page_number);

	/**
	 * The recognized text is returned as a char* which is coded in the same
	 * format as a box file used in training.
	 * Constructs coordinates in the original image - not just the rectangle.
	 * page_number is a 0-based page index that will appear in the box file.
	 * Returned string must be freed with the delete [] operator.
	 *
	 * @param page_number
	 * @return
	 */
	public native String getBoxText(int page_number);

	/**
	 * The recognized text is returned as a char* which is coded
	 * as UNLV format Latin-1 with specific reject and suspect codes.
	 * Returned string must be freed with the delete [] operator.
	 *
	 * @return
	 */
	public native String getUNLVText();

	/**
	 * Detect the orientation of the input image and apparent script (alphabet).
	 * orient_deg is the detected clockwise rotation of the input image in degrees
	 * (0, 90, 180, 270)
	 * orient_conf is the confidence (15.0 is reasonably confident)
	 * script_name is an ASCII string, the name of the script, e.g. "Latin"
	 * script_conf is confidence level in the script
	 * Returns true on success and writes values to each parameter as an output
	 *
	 * @param orient_deg
	 * @param orient_conf
	 * @param script_name
	 * @param script_conf
	 * @return
	 */
	public native  boolean detectOrientationScript(int[] orient_deg, float[] orient_conf, String script_name, float[] script_conf);

	/**
	 * The recognized text is returned as a char* which is coded
	 * as UTF8 and must be freed with the delete [] operator.
	 * page_number is a 0-based page index that will appear in the osd file.
	 *
	 * @param page_number
	 * @return
	 */
	public native String getOsdText(int page_number);

	/** Returns the (average) confidence value between 0 and 100. 
	 * 
	 * @return
	 */
	public native int meanTextConf();
	/**
	 * Returns all word confidences (between 0 and 100) in an array, terminated
	 * by -1.  The calling function must delete [] after use.
	 * The number of confidences should correspond to the number of space-
	 * delimited words in GetUTF8Text.
	 * 
	 * @return
	 */
	public native int[] allWordConfidences();

	/**
	 * Free up recognition results and any stored image data, without actually
	 * freeing any recognition data that would be time-consuming to reload.
	 * Afterwards, you must call SetImage or TesseractRect before doing
	 * any Recognize or Get* operation.
	 */
	public native void clear();

	/**
	 * Close down tesseract and free up all memory. End() is equivalent to
	 * destructing and reconstructing your TessBaseAPI.
	 * Once End() has been used, none of the other API functions may be used
	 * other than Init and anything declared above it in the class definition.
	 */
	public native void end();

	/**
	 * Clear any library-level memory caches.
	 * There are a variety of expensive-to-load constant data structures (mostly
	 * language dictionaries) that are cached globally -- surviving the Init()
	 * and End() of individual TessBaseAPI's.  This function allows the clearing
	 * of these caches.
	 **/
	public native static void clearPersistentCache();

	/**
	 * Check whether a word is valid according to Tesseract's language model
	 * @return 0 if the word is invalid, non-zero if valid.
	 * @warning temporary! This function will be removed from here and placed
	 * in a separate API at some future time.
	 *
	 * @param word
	 * @return
	 */
	public native boolean isValidWord(String word);

	/**
	 * Returns true if utf8_character is defined in the UniCharset.
	 * 
	 * TODO: Java's char is 16 bits while C/C++ is 8 bits
	 * 
	 * @param utf8_character
	 * @return
	 */
	public native boolean isValidCharacter(char utf8_character);

	/**
	 * 
	 * @param out_offset
	 * @param out_slope
	 * @return
	 */
	public native boolean getTextDirection(int[] out_offset, float[] out_slope);

	/** Sets Dict::letter_is_okay_ function to point to the given function. */
	//void SetDictFunc(DictFunc f);

	/** Sets Dict::probability_in_context_ function to point to the given
	 * function.
	 */
	//void SetProbabilityInContextFunc(ProbabilityInContextFunc f);

	/**
	 * Estimates the Orientation And Script of the image.
	 * @return true if the image was processed successfully.
	 */
	//bool DetectOS(OSResults*);

	/**
	 * Return text orientation of each block as determined by an earlier run
	 * of layout analysis.
	 *
	 * @param block_orientation
	 * @param vertical_writing
	 */
	public native void getBlockTextOrientations(int[][] block_orientation, boolean[][] vertical_writing);

	/** Sets Wordrec::fill_lattice_ function to point to the given function. */
	//public native void setFillLatticeFunc(FillLatticeFunc f);

	/** Find lines from the image making the BLOCK_LIST. */
	//public native BLOCK_LIST* findLinesCreateBlockList();

	/**
	 * Delete a block list.
	 * This is to keep BLOCK_LIST pointer opaque
	 * and let go of including the other headers.
	 */
	//public native static void deleteBlockList(BLOCK_LIST* block_list);

	/** Returns a ROW object created from the input row specification. */
	//public native static ROW *MakeTessOCRRow(float baseline, float xheight, float descender, float ascender);

	/** Returns a TBLOB corresponding to the entire input image. */
	//public native static TBLOB *MakeTBLOB(Pix *pix);

	/**
	 * This method baseline normalizes a TBLOB in-place. The input row is used
	 * for normalization. The denorm is an optional parameter in which the
	 * normalization-antidote is returned.
	 */
	//public native static void NormalizeTBLOB(TBLOB *tblob, ROW *row, bool numeric_mode);

	/** This method returns the features associated with the input image. */
	//public native void GetFeaturesForBlob(TBLOB* blob, INT_FEATURE_STRUCT* int_features, int* num_features, int* feature_outline_index);

	/**
	 * This method returns the row to which a box of specified dimensions would
	 * belong. If no good match is found, it returns nullptr.
	 */
	//public native static ROW* FindRowForBox(BLOCK_LIST* blocks, int left, int top, int right, int bottom);

	/**
	 * Method to run adaptive classifier on a blob.
	 * It returns at max num_max_matches results.
	 */
	//public native void RunAdaptiveClassifier(TBLOB* blob, int num_max_matches, int* unichar_ids, float* ratings, int* num_matches_returned);

	/** This method returns the string form of the specified unichar. 
	 * 
	 * @param unichar_id
	 * @return
	 */
	public native String getUnichar(int unichar_id);

	/** Return the pointer to the i-th dawg loaded into tesseract_ object. */
	//public native const Dawg *GetDawg(int i);

	/** Return the number of dawgs loaded into tesseract_ object.
	 * 
	 * @return
	 */
	public native int numDawgs();

	//public native Tesseract* tesseract() const { return tesseract_; }

	//public native OcrEngineMode oem() const { return last_oem_requested_; }

	//public native void InitTruthCallback(TruthCallback *cb) { truth_cb_ = cb; }
	
	/**
	 * 
	 * @param margin
	 */
	public native void setMinOrientationMargin(double margin);

	/**
	 * 
	 * 
	 */ 
	public JTessBaseAPI()
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
