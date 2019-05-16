#include "jtesseract_JTesseract.h"

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <clocale>

using namespace cv;
using namespace tesseract;

class JTesseractParams{
public:
	OcrEngineMode engine;
	String datapath;
	PageSegMode segementation;
	String lang;
	String imagePath;
	Mat image;
};

TessBaseAPI *ocr;
JTesseractParams params;

/*
 * Class:     tesseract_Tesseract
 * Method:    initialise
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_initialise(JNIEnv *env, jobject obj, jstring datapath, jstring lang, jint engine)
{
	jboolean copy = JNI_TRUE;
	params.datapath = String(env->GetStringUTFChars(datapath, &copy));
	params.lang = String(env->GetStringUTFChars(lang, &copy));
	
	if(engine == 0)
	{
		params.engine = OEM_TESSERACT_ONLY;
	} else if(engine == 1)
	{
		params.engine = OEM_LSTM_ONLY;
	} else if(engine == 2)
	{
		params.engine = OEM_TESSERACT_LSTM_COMBINED;
	} else if(engine == 3)
	{
		params.engine = OEM_DEFAULT;
	} else if(engine == 4)
	{
		params.engine = OEM_COUNT;
	} 
	
	ocr->Init(params.datapath.size() > 0 ? params.datapath.c_str() : NULL, params.lang.c_str(), params.engine);
}

/*
 * Class:     tesseract_Tesseract
 * Method:    setPageSegMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_setPageSegMode(JNIEnv *env, jobject obj, jint segMode)
{
	if(segMode == 0){
		params.segementation = PageSegMode::PSM_OSD_ONLY;
	} else if(segMode == 1){
		params.segementation = PageSegMode::PSM_AUTO_OSD;
	} else if(segMode == 2){
		params.segementation = PageSegMode::PSM_AUTO_ONLY;
	} else if(segMode == 3){
		params.segementation = PageSegMode::PSM_AUTO;
	} else if(segMode == 4){
		params.segementation = PageSegMode::PSM_SINGLE_COLUMN;
	} else if(segMode == 5){
		params.segementation = PageSegMode::PSM_SINGLE_BLOCK_VERT_TEXT;
	} else if(segMode == 6){
		params.segementation = PageSegMode::PSM_SINGLE_BLOCK;
	} else if(segMode == 7){
		params.segementation = PageSegMode::PSM_SINGLE_LINE;
	} else if(segMode == 8){
		params.segementation = PageSegMode::PSM_SINGLE_CHAR;
	} else if(segMode == 9){
		params.segementation = PageSegMode::PSM_SPARSE_TEXT;
	} else if(segMode == 9){
		params.segementation = PageSegMode::PSM_SPARSE_TEXT_OSD;
	} else if(segMode == 9){
		params.segementation = PageSegMode::PSM_RAW_LINE;
	} else if(segMode == 9){
		params.segementation = PageSegMode::PSM_COUNT;
	} else {
		params.segementation = PageSegMode::PSM_AUTO;
	}

	ocr->SetPageSegMode(params.segementation);
}

/*
 * Class:     tesseract_Tesseract
 * Method:    setImage
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_setImage(JNIEnv *env, jobject obj, jstring filename)
{
	String f_name(env->GetStringUTFChars(filename, NULL));
	params.image = imread(f_name, IMREAD_COLOR);
	ocr->SetImage(params.image.data, params.image.cols, params.image.rows, params.image.channels(), params.image.step);
}

/*
 * Class:     tesseract_Tesseract
 * Method:    getUTF8Text
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTesseract_getUTF8Text(JNIEnv *env, jobject obj)
{
	String text(ocr->GetUTF8Text());
	jstring result;
	result = env->NewStringUTF(text.c_str()); 

	return result;
}

JNIEXPORT jstring JNICALL Java_jtesseract_JTesseract_Version(JNIEnv *env, jclass type)
{
	String text(TessBaseAPI::Version());
	jstring result;
	result = env->NewStringUTF(text.c_str()); 

	return result;
}

/*
 * Class:     tesseract_Tesseract
 * Method:    clean
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_clean(JNIEnv *env, jobject obj)
{
	delete ocr;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    create
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_create(JNIEnv *env, jobject obj)
{
	std::setlocale(LC_ALL, "C");
	ocr = new TessBaseAPI();
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setFromBufferedImage
 * Signature: (Ljava/awt/image/BufferedImage;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_setFromBufferedImage
		(JNIEnv *env, jobject obj, jbyteArray data, jint rows, jint cols, jint channels)
{
	jbyte *dt = env->GetByteArrayElements(data, 0);	
	params.image = 	Mat (rows, cols, CV_8UC3, (uchar *)dt);
	ocr->SetImage(params.image.data, params.image.cols, params.image.rows, 3, params.image.step);
	
}
