//
//  ofxOpenNI2.cpp
//
//  Created by Niisato Hirotaka on 13/02/10.
//
#include <iostream>
#include "ofxOpenNI2.h"

using namespace openni;


void ofxOpenNI2::setup() {
    Status rc = openni::OpenNI::initialize();
    if (rc != openni::STATUS_OK) {
        cout << "Failed to initialize OpenNI:" << openni::OpenNI::getExtendedError() << endl;
        return;
        //return rc;
    }
    
    const char* deviceUri = openni::ANY_DEVICE;
   
 
    rc = device.open(deviceUri);
    if (rc != openni::STATUS_OK) {
        cout << "Failed to open device:" << openni::OpenNI::getExtendedError() << endl;
        return;
        // return rc;
    }
    
    
    //device.setDepthColorSyncEnabled(true);

    
    rc = depthStream.create(device, openni::SENSOR_DEPTH);
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't find depth stream:" << openni::OpenNI::getExtendedError() << endl;
        return;
    }

    
    const Array<VideoMode> &depthArray = depthStream.getSensorInfo().getSupportedVideoModes();
    cout << "depth modes: " << endl;
    for (int i=0;i<depthArray.getSize();i++) {
        const VideoMode &mode(depthArray[i]);
        cout << '\t' << i << ": " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    }

    depthStream.setVideoMode(depthArray[4]); // 640x480
    VideoMode mode(depthStream.getVideoMode());
    depthWidth = mode.getResolutionX();
    depthHeight = mode.getResolutionY();
    // depthPixels.allocate(depthWidth, depthHeight,OF_IMAGE_GRAYSCALE);
    depthTexture.allocate(depthWidth, depthHeight, GL_LUMINANCE16);
    cout << "depthMode: " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    
    rc = depthStream.start();
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't start depth stream:" << openni::OpenNI::getExtendedError() << endl;
        depthStream.destroy();
        return;
    }
    
    rc = colorStream.create(device, openni::SENSOR_COLOR);
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't find color stream:" << openni::OpenNI::getExtendedError() << endl;
        return;
    }
    

    const Array<VideoMode> &colorArray = colorStream.getSensorInfo().getSupportedVideoModes();
    cout << "color modes: " << endl;
    for (int i=0;i<colorArray.getSize();i++) {
        const VideoMode &mode(colorArray[i]);
        cout << '\t' << i << ": " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    }

    
    colorStream.setVideoMode(colorArray[9]); // 1280x1024
    
    mode = VideoMode(colorStream.getVideoMode());
    colorWidth = mode.getResolutionX();
    colorHeight = mode.getResolutionY();
    // colorPixels.allocate(colorWidth, colorHeight, OF_IMAGE_COLOR);
    colorTexture.allocate(colorWidth, colorHeight, GL_RGB);

    cout << "colorMode: " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    
    rc = colorStream.start();
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't start color stream:" << openni::OpenNI::getExtendedError() << endl;
        colorStream.destroy();
        return;
    }
    // bNewColor = false;
    // bNewDepth = false;

    //startThread();

}

void ofxOpenNI2::update() {

   

    VideoFrameRef       colorFrame; 
    colorStream.readFrame(&colorFrame);
    
    bNewColor = false;
    if ( colorFrame.isValid() ) {    
        //colorPixels.setFromPixels((unsigned char *)colorFrame.getData(), colorFrame.getWidth(), colorFrame.getHeight(), OF_IMAGE_COLOR);
        colorTexture.loadData((unsigned char *)colorFrame.getData(), colorFrame.getWidth(), colorFrame.getHeight(),GL_RGB);
        bNewColor = true;
    }

    VideoFrameRef       depthFrame;
    depthStream.readFrame(&depthFrame);

    bNewDepth=false;
    if ( depthFrame.isValid() ) {
        //depthPixels.setFromPixels((short unsigned int*)depthFrame.getData(), depthFrame.getWidth(), depthFrame.getHeight(), OF_IMAGE_GRAYSCALE);
        depthTexture.loadData((short unsigned int*)depthFrame.getData(), depthFrame.getWidth(), depthFrame.getHeight(),GL_LUMINANCE);
        bNewDepth = true;
    }

    /*

    if (bNewColor) {
        colorTexture.loadData(colorPixels);
        bNewColor = false;
    }

    if (bNewDepth) {
        depthTexture.loadData(depthPixels);
        bNewDepth = false;
    }
    */
}

void ofxOpenNI2::exit() {
    cout << "exit" << endl;
    //stopThread();
}

/*
void ofxOpenNI2::threadedFunction() {

    

     std::vector<openni::VideoStream*> streams;
     streams.push_back( &depthStream );
     streams.push_back( &colorStream );


    while (this->isThreadRunning()) {
        int changedIndex;
        openni::OpenNI::waitForAnyStream( &streams[0], streams.size(), &changedIndex ); 
        //if ( changedIndex == 0 ) {
            
            // ofxOpenNIScopedLock scopedLock(mutex);
            VideoFrameRef       colorFrame; 
            colorStream.readFrame(&colorFrame);
            
            if ( colorFrame.isValid() ) {
                
                colorPixels.setFromPixels((unsigned char *)colorFrame.getData(), colorFrame.getWidth(), colorFrame.getHeight(), OF_IMAGE_COLOR);
                bNewColor = true;
            }

            VideoFrameRef       depthFrame;
            depthStream.readFrame(&depthFrame);

            if ( depthFrame.isValid() ) {
                depthPixels.setFromPixels((short unsigned int*)depthFrame.getData(), depthFrame.getWidth(), depthFrame.getHeight(), OF_IMAGE_GRAYSCALE);
                bNewDepth = true;
            }
        // }
    }

    cout << "thread terminated" << endl;
}


void ofxOpenNI2::checkStream() {
    ofxOpenNIScopedLock scopedLock(mutex);
    
    // check depth stream
    if (useDepth) {
        depthStream.readFrame(&depthFrame);
        calculateHistogram(depthHist, MAX_DEPTH, depthFrame);

        const openni::DepthPixel* depthRow = (const openni::DepthPixel*)depthFrame.getData();
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++, depthRow++){
                depthPixels.setColor(x, y, *depthRow);
            }
        }

        const openni::DepthPixel* depth = (const openni::DepthPixel*)depthFrame.getData();
        for (int y = depthFrame.getCropOriginY(); y < depthFrame.getHeight() + depthFrame.getCropOriginY(); y++){
            unsigned char * texture = depthPixels.getPixels() + y * depthFrame.getWidth() * 4 + depthFrame.getCropOriginX() * 4;
            for (int x = 0; x < depthPixels.getWidth(); x++, depth++, texture += 4) {
                int nHistValue = depthHist[*depth];
                if (*depth != 0){
                    texture[0] = nHistValue;
                    texture[1] = nHistValue;
                    texture[2] = 0;
                }
            }
        }
    }

}






void ofxOpenNI2::calculateHistogram(float* histogram, int histogramSize, const openni::VideoFrameRef& frame) {
    const openni::DepthPixel* pDepth = (const openni::DepthPixel*)frame.getData();
    memset(histogram, 0, histogramSize*sizeof(float));
    int restOfRow = frame.getStrideInBytes() / sizeof(openni::DepthPixel) - frame.getWidth();
    int height = frame.getHeight();
    int width = frame.getWidth();
    
    unsigned int nNumberOfPoints = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x, ++pDepth) {
            if (*pDepth != 0) {
                histogram[*pDepth]++;
                nNumberOfPoints++;
            }
        }
        pDepth += restOfRow;
    }
    
    for (int nIndex=1; nIndex<histogramSize; nIndex++) {
        histogram[nIndex] += histogram[nIndex-1];
    }
    
    if (nNumberOfPoints) {
        for (int nIndex=1; nIndex<histogramSize; nIndex++) {
            histogram[nIndex] = (256 * (1.0f - (histogram[nIndex] / nNumberOfPoints)));
        }
    }
}



ofPixels ofxOpenNI2::getColorPixels() {
    ofxOpenNIScopedLock scopedLock(mutex);
    return colorPixels;
}

ofTexture ofxOpenNI2::getColorTexture() {
    ofxOpenNIScopedLock scopedLock(mutex);
    colorTexture.loadData(colorPixels);
    return colorTexture;
}

ofPixels ofxOpenNI2::getDepthPixels() {
    ofxOpenNIScopedLock scopedLock(mutex);
    return depthPixels;
}

ofTexture ofxOpenNI2::getDepthTexture() {
    ofxOpenNIScopedLock scopedLock(mutex);
    depthTexture.loadData(depthPixels.getPixels(), width, height, GL_RGBA);
    return depthTexture;
}


float ofxOpenNI2::getDepth(int x, int y) {
    ofxOpenNIScopedLock scopedLock(mutex);
    if (x > width || y > height || x < 0 || y < 0) return 0;
    return depthPixels[depthPixels.getPixelIndex(x, y)];
}

*/