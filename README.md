lightweight openFrameworks addons for OpenNI 2.x 
==================================================
The ofxOpenNI2 is a lightweight wrapper for the OpenNI 2.x (NiTE 2 is not included).

OSX guidelines:

1. brew install openni2 --universal (openframeworks 0.84 support only 32 bit on osx) 
2. edit your Project.xconfig:
OTHER_LDFLAGS = $(OF_CORE_LIBS) "/usr/local/lib/ni2/libOpenNI2.dylib"
HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) "/usr/local/include/ni2"
3. you don't need to copy the dylib to your bin path because it is installed on your system

linux64
for ubuntu 13.10 I'd needed to build OpenNi2 from source from develop branch (http://stackoverflow.com/questions/19938830/openni2-and-asus-xtion-pro-live-device-not-found)
connect to usb2 port (not usb3)
because of some naming conflicts you need to include ofxOpenNI2 before ofMain.h both in main.cpp and ofApp.h
copy linux64 Redist files (under lib) to app bin folder
compile
run from app path (ie ./bin/blabla)

for ubuntu 14.04 and xtion pro
update your cameras firmware from asus website on windows machine to fix usb3 problems
push the source code (structure.io links to github)
checkout develop branch
install dependencies include java to build redist package
make

