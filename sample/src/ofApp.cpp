#include "ofApp.h"

#define CAM_WIDTH 640
#define CAM_HEIGHT 480


//--------------------------------------------------------------
void ofApp::setup(){
//    const char* version = (const char*)glGetString(GL_VERSION);
//    cout << version << endl;
//    shader.load("shaders/shader.vert", "shaders/shader.frag");
    ofEnableAlphaBlending();
    
    cam.setup();
    cam.setDepthMode(0);
    cam.setColorMode(0);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
   
    cam.depthTexture.draw(0,0);
    cam.colorTexture.draw(0,cam.depthHeight);
    
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
