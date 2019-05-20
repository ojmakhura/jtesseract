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

#include "jtesseract_JTessBaseAPI.h"

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/strngs.h>
#include <tesseract/genericvector.h>
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
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_initialise(JNIEnv *env, jobject obj, jstring datapath, jstring lang, jint engine)
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
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setPageSegMode(JNIEnv *env, jobject obj, jint segMode)
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
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setImage(JNIEnv *env, jobject obj, jstring filename)
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
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getUTF8Text(JNIEnv *env, jobject obj)
{
	String text(ocr->GetUTF8Text());
	jstring result;
	result = env->NewStringUTF(text.c_str()); 

	return result;
}

JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_Version(JNIEnv *env, jclass type)
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
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_clean(JNIEnv *env, jobject obj)
{
	delete ocr;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    create
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_create(JNIEnv *env, jobject obj)
{
	std::setlocale(LC_ALL, "C");
	ocr = new TessBaseAPI();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    setImageHelper
 * Signature: ([BIII)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setImageHelper
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
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_catchSignals(JNIEnv *env, jclass type)
{
	ocr->CatchSignals();
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setInputName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setInputName(JNIEnv *env, jobject obj, jstring name)
{
	ocr->SetInputName(env->GetStringUTFChars(name, 0));
}


/*
 * Class:     jtesseract_JTesseract
 * Method:    getInputName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getInputName (JNIEnv *env, jobject obj)
{
	String name(ocr->GetInputName());
	return env->NewStringUTF(name.c_str());
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getSourceYResolution
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTessBaseAPI_getSourceYResolution(JNIEnv *env, jobject obj)
{
	jint res = ocr->GetSourceYResolution();

	return res;
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    getDatapath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getDatapath(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(ocr->GetDatapath());
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setOutputName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setOutputName(JNIEnv *env, jobject obj, jstring name)
{
	ocr->SetOutputName(env->GetStringUTFChars(name, 0));
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    setVariable
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_setVariable(JNIEnv *env, jobject obj, jstring name, jstring value)
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
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_setDebugVariable(JNIEnv *env, jobject obj, jstring name, jstring value)
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
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_getIntVariable(JNIEnv *env, jobject object, jstring name, jobject value)
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
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_getBoolVariable(JNIEnv *env, jobject onj, jstring name, jobject value)
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
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_getDoubleVariable(JNIEnv *env, jobject obj, jstring name, jobject value)
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
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getStringVariable(JNIEnv *env, jobject obj, jstring name)
{
	const char *res = ocr->GetStringVariable(env->GetStringUTFChars(name, 0));
	return env->NewStringUTF(res);
}

/*
 * Class:     jtesseract_JTesseract
 * Method:    printVariables
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_printVariables(JNIEnv *env, jobject obj, jstring filename)
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
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_getVariableAsString(JNIEnv *env, jobject obj, jstring name, jstring value)
{
	/*STRING str;
	if(ocr->GetVariableAsString(env->GetStringUTFChars(name, 0), str))
	{
		value = env->NewStringUTF(str.string());
		return true;
	}*/
	return false;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getInitLanguagesAsString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getInitLanguagesAsString(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(ocr->GetInitLanguagesAsString());
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getLoadedLanguagesAsVector
 * Signature: (Ljava/util/ArrayList;)V
 */
JNIEXPORT jobject JNICALL Java_jtesseract_JTessBaseAPI_getLoadedLanguagesAsVector(JNIEnv *env, jobject obj)
{
	GenericVector<STRING> langs;
	ocr->GetLoadedLanguagesAsVector(&langs);

	jclass v_class = env->FindClass("java/util/ArrayList");
	jmethodID init = env->GetMethodID(v_class, "<init>", "()V");

	if(init == NULL)
	{
		return NULL;
	}

	jobject list = env->NewObject(v_class, init);
	if(list == NULL)
	{
		return NULL;
	}

	jmethodID push_back = env->GetMethodID(v_class, "push_back", "(Ljava/lang/String)V");
	if(push_back == NULL)
	{
		return NULL;
	}

	int size = langs.size();
	for(int i = 0; i < size; i++)
	{
		STRING str = langs[i];
		env->CallVoidMethod(list, push_back, env->NewStringUTF(str.string()));
	}

	return list;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getAvailableLanguagesAsVector
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_jtesseract_JTessBaseAPI_getAvailableLanguagesAsVector(JNIEnv *env, jobject obj)
{
	GenericVector<STRING> langs;
	ocr->GetAvailableLanguagesAsVector(&langs);

	jclass v_class = env->FindClass("java/util/ArrayList");
	jmethodID init = env->GetMethodID(v_class, "<init>", "()V");

	if(init == NULL)
	{
		return NULL;
	}

	jobject list = env->NewObject(v_class, init);
	if(list == NULL)
	{
		return NULL;
	}

	jmethodID push_back = env->GetMethodID(v_class, "push_back", "(Ljava/lang/String)V");
	if(push_back == NULL)
	{
		return NULL;
	}

	int size = langs.size();
	for(int i = 0; i < size; i++)
	{
		STRING str = langs[i];
		env->CallVoidMethod(list, push_back, env->NewStringUTF(str.string()));
	}

	return list;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    initLangMod
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTessBaseAPI_initLangMod(JNIEnv *env, jobject obj, jstring datapath, jstring language)
{
	return ocr->InitLangMod(env->GetStringUTFChars(datapath, 0), env->GetStringUTFChars(language, 0));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    initForAnalysePage
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_initForAnalysePage(JNIEnv *env, jobject obj)
{
	ocr->InitForAnalysePage();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    readConfigFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_readConfigFile(JNIEnv *env, jobject obj, jstring filename)
{
	ocr->ReadConfigFile(env->GetStringUTFChars(filename, 0));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    readDebugConfigFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_readDebugConfigFile(JNIEnv *env, jobject obj, jstring filename)
{
	ocr->ReadDebugConfigFile(env->GetStringUTFChars(filename, 0));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    tesseractRect
 * Signature: (Ljava/lang/String;IIIIII)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_tesseractRect(JNIEnv *env, jobject obj, jbyteArray imagedata,
						jint bytes_per_pixel, jint bytes_per_line, jint left, jint top, jint width, jint height)
{
	unsigned char *data = (unsigned char *)env->GetByteArrayElements(imagedata, 0);
	char *ch = ocr->TesseractRect(data, bytes_per_pixel, bytes_per_line, left, top, width, height);

	return env->NewStringUTF(ch);
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    clearAdaptiveClassifier
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_clearAdaptiveClassifier(JNIEnv *env, jobject obj)
{
	ocr->ClearAdaptiveClassifier();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    setSourceResolution
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setSourceResolution(JNIEnv *env, jobject obj, jint ppi)
{
	ocr->SetSourceResolution(ppi);
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    setRectangle
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setRectangle(JNIEnv *env, jobject obj, jint left, jint top, jint width, jint height)
{
	ocr->SetRectangle(left, top, width, height);
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    GetRegions
 * Signature: ()[Ljtesseract/Box;
 */
JNIEXPORT jobject JNICALL Java_jtesseract_JTessBaseAPI_getRegions(JNIEnv *env, jobject obj)
{
	Pixa *pxs;
	Boxa *boxes = ocr->GetRegions(&pxs);

	jclass lst_class = env->FindClass("java/util/ArrayList");
	jmethodID bxs_init = env->GetMethodID(lst_class, "<init>", "()V");

	if(bxs_init == NULL)
	{
		return NULL;
	}

	jobject list = env->NewObject(lst_class, bxs_init);
	if(list == NULL)
	{
		return NULL;
	}

	jmethodID push_back = env->GetMethodID(lst_class, "push_back", "(Ljava/lang/Object)V");
	if(push_back == NULL)
	{
		return NULL;
	}

	for(int i = 0; i < boxes->n; i++)
	{
		Box *bx = boxes->box[i];

		jclass bxs_class = env->FindClass("jtesseract/Box");
		jmethodID bxs_init = env->GetMethodID(bxs_class, "<init>", "(IIII)V");
		jobject box = env->NewObject(bxs_class, bxs_init, bx->x, bx->y, bx->w, bx->h);

		env->CallVoidMethod(list, push_back, box);
	}

	return list;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getThresholdedImageScaleFactor
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTessBaseAPI_getThresholdedImageScaleFactor(JNIEnv *env, jobject obj)
{
	return ocr->GetThresholdedImageScaleFactor();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getTSVText
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getTSVText(JNIEnv *env, jobject obj, jint page_number)
{
	return env->NewStringUTF(ocr->GetTSVText(page_number));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getBoxText
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getBoxText(JNIEnv *env, jobject obj, jint page_number)
{
	return env->NewStringUTF(ocr->GetBoxText(page_number));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getUNLVText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getUNLVText(JNIEnv *env, jobject obj)
{
	return env->NewStringUTF(ocr->GetUNLVText());
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    fetectOrientationScript
 * Signature: ([I[F[Ljava/lang/String;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_detectOrientationScript(JNIEnv *env, jobject obj,
						jintArray orient_deg, jfloatArray orient_conf, jstring script_name, jfloatArray script_conf)
{

	return false;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getOsdText
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getOsdText(JNIEnv *env, jobject obj, jint page_number)
{
	char *res = ocr->GetOsdText(page_number);
	return env->NewStringUTF(res);
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    meanTextConf
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTessBaseAPI_meanTextConf(JNIEnv *env, jobject obj)
{
	return ocr->MeanTextConf();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    AllWordConfidences
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_jtesseract_JTessBaseAPI_allWordConfidences(JNIEnv *env, jobject obj)
{
	int* res = ocr->AllWordConfidences();
	int size = sizeof(res)/sizeof(*res);
	jintArray ret = env->NewIntArray(size);
	env->SetIntArrayRegion(ret, 0, size, res);

	return ret;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_clear(JNIEnv *env, jobject obj)
{
	ocr->Clear();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    end
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_end(JNIEnv *env, jobject obj)
{
	ocr->End();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    clearPersistentCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_clearPersistentCache(JNIEnv *env, jclass cls)
{
	TessBaseAPI::ClearPersistentCache();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    isValidWord
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_isValidWord(JNIEnv *env, jobject obj, jstring word)
{
	return ocr->IsValidWord(env->GetStringUTFChars(word, 0));
}

/**
 * 
 * TODO: Java's char is 16 bits while C/C++ is 8 bits
 * 
 * Class:     jtesseract_JTessBaseAPI
 * Method:    IsValidCharacter
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_isValidCharacter(JNIEnv *env, jobject obj, jbyte utf8_character)
{
	const char ch = (unsigned char)utf8_character;
	if(ocr->IsValidCharacter(&ch))
	{
		return true;
	}

	return false;
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getTextDirection
 * Signature: ([I[F)Z
 */
JNIEXPORT jboolean JNICALL Java_jtesseract_JTessBaseAPI_getTextDirection(JNIEnv *env, jobject obj, jintArray out_offset, jfloatArray out_slope)
{
	jint *offset = env->GetIntArrayElements(out_offset, NULL);
	jfloat *slope = env->GetFloatArrayElements(out_slope, NULL);

	return ocr->GetTextDirection(offset, slope);
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getBlockTextOrientations
 * Signature: ([[I[[Z)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_getBlockTextOrientations(JNIEnv *env, jobject obj, jobjectArray block_orientation, jobjectArray vertical_writing)
{
	//int b_size = env->GetArrayLength(myArray);
	//jintArray blocks = 
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    getUnichar
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jtesseract_JTessBaseAPI_getUnichar(JNIEnv *env, jobject obj, jint unichar_id)
{
	return env->NewStringUTF(ocr->GetUnichar(unichar_id));
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    numDawgs
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jtesseract_JTessBaseAPI_numDawgs(JNIEnv *env, jobject obj)
{
	return ocr->NumDawgs();
}

/*
 * Class:     jtesseract_JTessBaseAPI
 * Method:    setMinOrientationMargin
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_jtesseract_JTessBaseAPI_setMinOrientationMargin(JNIEnv *env, jobject obj, jdouble margin)
{
	ocr->set_min_orientation_margin(margin);
}