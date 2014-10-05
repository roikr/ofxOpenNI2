//
//  ofxOpenNI2.h
//
//  Created by Niisato Hirotaka on 13/02/11.
//

#ifndef emptyExample_ofxOpenNI2_h
#define emptyExample_ofxOpenNI2_h

//#include "NiTE.h"
#include "OpenNI.h"
// #include "ofThread.h"
#include "ofTexture.h"
// #include "ofPixels.h"
// #include <map>
// #include <vector>


// #define MAX_DEPTH 10000
// #define MAX_USERS 10

class ofxOpenNI2 { //: public ofThread {
private:
   
    
    
    
    openni::Device              device;
    openni::VideoStream         depthStream;  
    openni::VideoStream         colorStream;

    
    // ofShortPixels                depthPixels;
    // ofPixels                    colorPixels;
    
    // bool bNewColor;
    // bool bNewDepth;

    // float   depthHist[MAX_DEPTH];
    // void calculateHistogram(float* histogram, int histogramSize, const openni::VideoFrameRef& frame);
    
// protected:
    //void threadedFunction();
    // void checkStream();
    
    openni::VideoFrameRef       depthFrame;
    openni::VideoFrameRef        colorFrame;
    
    
    
public:
    
    struct mode {
        mode(int width,int height,int pixelFormat,int fps):width(width),height(height),pixelFormat(pixelFormat),fps(fps) {};
        int width;
        int height;
        int pixelFormat;
        int fps;
    };

    int colorHeight, colorWidth, depthHeight, depthWidth;
    
    static void init();
    static vector<string> listDevices();
    openni::VideoStream& getColorStream() { return colorStream;};
    openni::VideoStream& getDepthStream() { return depthStream;};
    
    void setup(string uri="");
    vector<mode> listDepthModes();
    void setDepthMode(int index);
    vector<mode> listColorModes();
    void setColorMode(int index);
    void setRegistrationMode(bool bMode);
    void update();
    void exit();
    

    
    
    short unsigned int *getDepth();
    unsigned char *getColor();
    ofVec3f getWorldCoordinateAt(int x, int y,unsigned int depth);
    
    
//    ofTexture                   depthTexture;
//    ofTexture                   colorTexture;
    
    bool bNewColor;
    bool bNewDepth;
    // float           getDepth(int x, int y);
    
    
};

/*
class ofxOpenNIScopedLock {
public:
    ofxOpenNIScopedLock(ofMutex & _mutex): mutex(_mutex){
        mutex.lock();
    };
    ~ofxOpenNIScopedLock(){
        mutex.unlock();
    };
    ofMutex & mutex;
};
*/
#endif
