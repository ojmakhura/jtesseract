/*
 * JTesseract.cpp
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

#include "jtesseract_JTesseract.h"

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <clocale>
#include <stdio.h>

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

/*
 * Class:     jtesseract_JTesseract
 * Method:    catchSignals
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_catchSignals(JNIEnv *env, jclass type)
{
	ocr->CatchSignals();
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setInputName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_setInputName(JNIEnv *env, jobject obj, jstring name)
{
	ocr->SetInputName(env->GetStringUTFChars(name, 0));
}


/*
 * Class:     jtesseract_JTesseract
 * Method:    getInputName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTesseract_getInputName (JNIEnv *env, jobject obj)
{
	String name(ocr->GetInputName());
	return env->NewStringUTF(name.c_str());
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getSourceYResolution
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTesseract_getSourceYResolution(JNIEnv *env, jobject obj)
{
	jint res = ocr->GetSourceYResolution();

	return res;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getDatapath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTesseract_getDatapath(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(ocr->GetDatapath());
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setOutputName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_setOutputName(JNIEnv *env, jobject obj, jstring name)
{
	ocr->SetOutputName(env->GetStringUTFChars(name, 0));
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setVariable
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_setVariable(JNIEnv *env, jobject obj, jstring name, jstring value)
{
	if(ocr->SetVariable(env->GetStringUTFChars(name, 0), env->GetStringUTFChars(value, 0)))
	{
		return true;
	} else {
		return false;
	}
}


/*
 * Class:     jtesseract_JTesseract
 * Method:    setDebugVariable
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_setDebugVariable(JNIEnv *env, jobject obj, jstring name, jstring value)
{
	if(ocr->SetDebugVariable(env->GetStringUTFChars(name, 0), env->GetStringUTFChars(value, 0)))
	{
		return true;
	} 
	
	return false;
}

/**
 * This method uses the jtesseract.J
 * 
 * Class:     jtesseract_JTesseract
 * Method:    getIntVariable
 * Signature: (Ljava/lang/String;Ljtesseract/JTessInt;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_getIntVariable(JNIEnv *env, jobject object, jstring name, jobject value)
{
	int result;
	
	if(ocr->GetIntVariable(env->GetStringUTFChars(name, 0), &result))
	{
		jclass v_class = env->GetObjectClass(value);
		jmethodID setValue = env->GetMethodID(v_class, "setValue", "(I)V");
		env->CallVoidMethod(v_class, setValue, result);
		return true;
	}

	return false;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getBoolVariable
 * Signature: (Ljava/lang/String;Ljtesseract/JTessBoolean;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_getBoolVariable(JNIEnv *env, jobject onj, jstring name, jobject value)
{
	bool result;
	
	if(ocr->GetBoolVariable(env->GetStringUTFChars(name, 0), &result))
	{
		jclass v_class = env->GetObjectClass(value);
		jmethodID setValue = env->GetMethodID(v_class, "setValue", "(Z)V");
		env->CallVoidMethod(v_class, setValue, result);
		return true;
	}

	return false;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getDoubleVariable
 * Signature: (Ljava/lang/String;Ljava/lang/Double;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_getDoubleVariable(JNIEnv *env, jobject obj, jstring name, jobject value)
{
	double result;
	
	if(ocr->GetDoubleVariable(env->GetStringUTFChars(name, 0), &result))
	{
		jclass v_class = env->GetObjectClass(value);
		jmethodID setValue = env->GetMethodID(v_class, "setValue", "(D)V");
		env->CallVoidMethod(v_class, setValue, result);
		return true;
	}

	return false;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getStringVariable
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTesseract_getStringVariable(JNIEnv *env, jobject obj, jstring name)
{
	const char *res = ocr->GetStringVariable(env->GetStringUTFChars(name, 0));
	return env->NewStringUTF(res);
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    printVariables
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTesseract_printVariables(JNIEnv *env, jobject obj, jstring filename)
{
	FILE* fp;
	fp = fopen(env->GetStringUTFChars(filename, 0), "w");
	ocr->PrintVariables(fp);
	fclose(fp);
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getVariableAsString
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTesseract_getVariableAsString(JNIEnv *env, jobject obj, jstring name, jstring value)
{
	if(ocr->GetVariableAsString(env->GetStringUTFChars(name, 0), env->GetStringUTFChars(value, 0)))
	return false;
}

