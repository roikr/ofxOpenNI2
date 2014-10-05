//
//  ofxOpenNI2.cpp
//
//  Created by Niisato Hirotaka on 13/02/10.
//
#include <iostream>
#include "ofxOpenNI2.h"

using namespace openni;

void ofxOpenNI2::init() {
    Status rc = openni::OpenNI::initialize();
    if (rc != openni::STATUS_OK) {
        cout << "Failed to initialize OpenNI:" << openni::OpenNI::getExtendedError() << endl;
        return;
        
    }
    
    

}

vector<string> ofxOpenNI2::listDevices() {
    openni::Array<openni::DeviceInfo> deviceList;
	openni::OpenNI::enumerateDevices(&deviceList);
    vector<string> devices;
    for (int i=0;i<deviceList.getSize();i++) {
        devices.push_back(deviceList[i].getUri());
    }
    
    return devices;
}


void ofxOpenNI2::setup(string uri) {
    
    Status rc;
    
    if (uri.empty()) {
        rc = device.open(openni::ANY_DEVICE);
    }
    else {
        rc = device.open(uri.c_str());
    }
    
 
     
    if (rc != openni::STATUS_OK) {
        cout << "Failed to open device:" << openni::OpenNI::getExtendedError() << endl;
        return;
        // return rc;
    }
    
    
    rc = depthStream.create(device, openni::SENSOR_DEPTH);
    if (rc != openni::STATUS_OK) {
        cout << "ofxOpenNI2: Couldn't find depth stream:" << openni::OpenNI::getExtendedError() << endl;
        
    } else
    
    rc = colorStream.create(device, openni::SENSOR_COLOR);
    if (rc != openni::STATUS_OK) {
        cout << "ofxOpenNI2: Couldn't find color stream:" << openni::OpenNI::getExtendedError() << endl;
        
    }


    

    
    bNewColor = false;
    bNewDepth = false;
    
    /*

    //startThread();

    
    */
}

vector<ofxOpenNI2::mode> ofxOpenNI2::listDepthModes() {
    const Array<VideoMode> &depthArray = depthStream.getSensorInfo().getSupportedVideoModes();
    vector<ofxOpenNI2::mode> modes;
    cout << "depth modes: " << endl;
    for (int i=0;i<depthArray.getSize();i++) {
        const VideoMode &mode(depthArray[i]);
        modes.push_back(ofxOpenNI2::mode(mode.getResolutionX(),mode.getResolutionY(),mode.getPixelFormat(),mode.getFps()));
        cout << '\t' << i << ": " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    }
    return modes;
}


void ofxOpenNI2::setDepthMode(int index) {
    depthStream.stop();
    const Array<VideoMode> &depthArray = depthStream.getSensorInfo().getSupportedVideoModes();
    depthStream.setVideoMode(depthArray[index]);
    
    VideoMode mode(depthStream.getVideoMode());
    depthWidth = mode.getResolutionX();
    depthHeight = mode.getResolutionY();
    // depthPixels.allocate(depthWidth, depthHeight,OF_IMAGE_GRAYSCALE);
    // GL_R16 will hint opengl to allocate with GL_UNSIGNED_SHORT
//    depthTexture.allocate(depthWidth, depthHeight, GL_R16 );  // GL_R16// GL_LUMINANCE16
    
    
    cout << "depthMode: " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    
    Status rc = depthStream.start();
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't start depth stream:" << openni::OpenNI::getExtendedError() << endl;
        depthStream.destroy();
        return;
    }
    
    

}

vector<ofxOpenNI2::mode> ofxOpenNI2::listColorModes() {
    const Array<VideoMode> &colorArray = colorStream.getSensorInfo().getSupportedVideoModes();
    vector<ofxOpenNI2::mode> modes;
    vector<pair<ofVec2f,ofVec2f> > list;
    cout << "color modes: " << endl;
    for (int i=0;i<colorArray.getSize();i++) {
        const VideoMode &mode(colorArray[i]);
        modes.push_back(ofxOpenNI2::mode(mode.getResolutionX(),mode.getResolutionY(),mode.getPixelFormat(),mode.getFps()));
        cout << '\t' << i << ": " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
        
    }
    return modes;
}


void ofxOpenNI2::setColorMode(int index) {
    colorStream.stop();
    const Array<VideoMode> &colorArray = colorStream.getSensorInfo().getSupportedVideoModes();
    colorStream.setVideoMode(colorArray[index]); // 9 = 1280x1024
    
    VideoMode mode = VideoMode(colorStream.getVideoMode());
    colorWidth = mode.getResolutionX();
    colorHeight = mode.getResolutionY();
    // colorPixels.allocate(colorWidth, colorHeight, OF_IMAGE_COLOR);
//    colorTexture.allocate(colorWidth, colorHeight, GL_RGB);
    
    cout << "colorMode: " << mode.getResolutionX() <<"x" << mode.getResolutionY() << ", " << mode.getPixelFormat() << ", " << mode.getFps() << endl;
    
    Status rc = colorStream.start();
    if (rc != openni::STATUS_OK) {
        cout << "SimpleViewer: Couldn't start color stream:" << openni::OpenNI::getExtendedError() << endl;
        colorStream.destroy();
        return;
    }
    
}

void ofxOpenNI2::setRegistrationMode(bool bMode) {
    //device.setDepthColorSyncEnabled(true);
    openni::ImageRegistrationMode mode = bMode ? openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR : openni::IMAGE_REGISTRATION_OFF;
    if (device.isImageRegistrationModeSupported(mode)) {
        cout << "getImageRegistrationMode: " << device.getImageRegistrationMode() << endl;
        Status rc = device.setImageRegistrationMode(mode);
        if (rc != openni::STATUS_OK) {
            cout << "setImageRegistrationMode " << mode << " has been failed: " << openni::OpenNI::getExtendedError() << endl;
        }
        cout << "getImageRegistrationMode: " << device.getImageRegistrationMode() << endl;
    }
}

void ofxOpenNI2::update() {

    
    colorStream.readFrame(&colorFrame);
    bNewColor = colorStream.isValid();
    
//    bNewColor = false;
//    if ( colorFrame.isValid() ) {
//        //colorPixels.setFromPixels((unsigned char *)colorFrame.getData(), colorFrame.getWidth(), colorFrame.getHeight(), OF_IMAGE_COLOR);
//        colorTexture.loadData((unsigned char *)colorFrame.getData(), colorFrame.getWidth(), colorFrame.getHeight(),GL_RGB);
//        bNewColor = true;
//    }
    
    depthStream.readFrame(&depthFrame);
    bNewDepth = depthFrame.isValid();
    
    /*
    bNewDepth=false;
    if ( depthFrame.isValid() ) {
//        cout << depthFrame.getStrideInBytes() << endl;
        //depthPixels.setFromPixels((short unsigned int*)depthFrame.getData(), depthFrame.getWidth(), depthFrame.getHeight(), OF_IMAGE_GRAYSCALE);

        // the magic is the short casting - which use GL_UNSIGNED_SHORT type
        depthTexture.loadData((short unsigned int*)depthFrame.getData(), depthFrame.getWidth(), depthFrame.getHeight(),GL_RED); // GL_RED,GL_LUMINANCE
        bNewDepth = true;
    }
     */
    
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

short unsigned int* ofxOpenNI2::getDepth() {
    return (short unsigned int*)depthFrame.getData();
}

unsigned char* ofxOpenNI2::getColor() {
    return (unsigned char*)colorFrame.getData();
}

ofVec3f ofxOpenNI2::getWorldCoordinateAt(int x, int y,unsigned int depth) {
    ofVec3f p;
    
    CoordinateConverter::convertDepthToWorld(depthStream, x, y, depth, &p.x, &p.y, &p.z);
    
    return p;
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