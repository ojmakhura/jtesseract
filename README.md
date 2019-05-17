# jtesseract
A java wrapper for tesseract OCR library. The library has simplified build with cmake and maven. The main requirement for this library is the installation of tesseract and opencv.

## CMake
With cmake, clone the repo, and build in the normal way.

## maven
If you are using maven, you need to inspect pom.xml at the locations where the maven-native plugin is defined. Change the include directories to match your system as well as the linked libraries. The current setting uses OpenCV 4 that was compiled with Cuda.

Only 4 of the methods have been properly tested. If you need to use the other ones and are not working, drop an issue or a pull requiest.