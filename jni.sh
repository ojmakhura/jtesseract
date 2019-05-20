echo "Compiling JTesseract.java"
CURR_DIR=`pwd`
echo $CURR_DIR
cd src/main/java
javac jtesseract/JTessBaseAPI.java

echo "Running javah"
javah -jni -d ../native jtesseract.JTessBaseAPI

echo "Cleaning up ..."
rm jtesseract/*.class
cd $CURR_DIR