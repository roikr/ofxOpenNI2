#include "ofApp.h"

#define CAM_WIDTH 640
#define CAM_HEIGHT 480


//--------------------------------------------------------------
void ofApp::setup(){
//    const char* version = (const char*)glGetString(GL_VERSION);
//    cout << version << endl;
//    shader.load("shaders/shader.vert", "shaders/shader.frag");
    ofEnableAlphaBlending();
    ofDisableArbTex();
    ofxOpenNI2::init();
    
    cam.setup();
    cam.setDepthMode(5);
    cam.setColorMode(9);

    tex.allocate(cam.colorWidth, cam.colorHeight, GL_RGB );
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    
    if(cam.bNewColor) {
        tex.loadData(cam.getColor(), cam.colorWidth, cam.colorHeight,GL_RGB);
        
        
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    tex.draw(0,0);
    //cam.depthTexture.draw(0,0);
    //cam.colorTexture.draw(0,cam.depthHeight);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
