echo "Compiling JTesseract.java"
CURR_DIR=`pwd`
cd src/main/java
javac jtesseract/JTesseract.java

echo "Running javah"
javah -jni -d ../native jtesseract.JTesseract
