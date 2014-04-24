lightweight openFrameworks addons for OpenNI 2.x 
==================================================
The ofxOpenNI2 is a lightweight wapper for the OpenNI 2.x, NiTE 2.

guidelines:

1. download openNI2 sdk from primesense
2. copy include dir to addons
3. copy files under redist to bin folder
4. works only with xcode
5. 

linux64
for ubuntu 13.10 I'd needed to build OpenNi2 from source from develop branch (http://stackoverflow.com/questions/19938830/openni2-and-asus-xtion-pro-live-device-not-found)
connect to usb2 port (not usb3)
because of some naming conflicts you need to include ofxOpenNI2 before ofMain.h both in main.cpp and ofApp.h
copy linux64 Redist files (under lib) to app bin folder
compile
run from app path (ie ./bin/blabla)
