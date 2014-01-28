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
    
public:

    int colorHeight, colorWidth, depthHeight, depthWidth;
    
    
    void setup();
    void listDepthModes();
    void setDepthMode(int index);
    void listColorModes();
    void setColorMode(int index);
    void update();
    void exit();
    
    ofTexture                   depthTexture;
    ofTexture                   colorTexture;
    
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
