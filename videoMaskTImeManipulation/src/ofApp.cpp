#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	frameByframe = false;

	// Uncomment this to show movies with alpha channels
	// fingerMovie.setPixelFormat(OF_PIXELS_RGBA);

	fingerMovie.load("movies/zabriskie.mov");
    fingerMovie.setLoopState(OF_LOOP_NORMAL);
    fingerMovie.play();
    
    alteredMovie.load("movies/zabriskie.mov");
    alteredMovie.setLoopState(OF_LOOP_NORMAL);
	alteredMovie.play();
    
//    path.lineTo(...);
    // draw on the path, level of gray means alpha
    
    img.load("koala.jpg");

    
    fbo.allocate(640,360,GL_RGBA);
//    fbo.allocate(640,360,GL_LUMINANCE); //or GL_RED if you are using the programmable renderer
    fbo.begin();
    ofClear(0,0,0,0);
        ofSetColor(255, 255, 255, 255);
        ofDrawEllipse(320,180,350, 200);
    fbo.end();

    img.getTexture().setAlphaMask(fbo.getTexture());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.update();
    alteredMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    

	ofSetHexColor(0xFFFFFF);
    
    alteredMovie.draw(0,20);
    
    ofTexture myFrame = fingerMovie.getTexture();
    
    myFrame.setAlphaMask(fbo.getTexture());

    
//    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    myFrame.draw(0,20);
    
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
//    ofDisableBlendMode();
    
    
    fbo.draw(640, 400);
    img.draw(0, 400);
    
    
    
//    ofSetHexColor(0x000000);
//    ofPixels & pixels = fingerMovie.getPixels();
//
//    int vidWidth = pixels.getWidth();
//    int vidHeight = pixels.getHeight();
//    int nChannels = pixels.getNumChannels();
//
//    // let's move through the "RGB(A)" char array
//    // using the red pixel to control the size of a circle.
//    for (int i = 4; i < vidWidth; i+=8){
//        for (int j = 4; j < vidHeight; j+=8){
//            unsigned char r = pixels[(j * 320 + i)*nChannels];
//            float val = 1 - ((float)r / 255.0f);
//			ofDrawCircle(400 + i,20+j,10*val);
//        }
//    }


    ofSetHexColor(0x000000);
	ofDrawBitmapString("press f to change",20,420);
    if(frameByframe) ofSetHexColor(0xCCCCCC);
    ofDrawBitmapString("mouse speed position",20,440);
    if(!frameByframe) ofSetHexColor(0xCCCCCC); else ofSetHexColor(0x000000);
    ofDrawBitmapString("keys <- -> frame by frame " ,190,440);
    ofSetHexColor(0x000000);

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/"+ofToString(fingerMovie.getTotalNumFrames()),20,480);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition()*fingerMovie.getDuration(),2) + "/"+ofToString(fingerMovie.getDuration(),2),20,500);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(),2),20,520);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,560);

    if(fingerMovie.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie",20,540);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case 'f':
            frameByframe=!frameByframe;
            fingerMovie.setPaused(frameByframe);
        break;
        case OF_KEY_LEFT:
            fingerMovie.previousFrame();
        break;
        case OF_KEY_RIGHT:
            fingerMovie.nextFrame();
        break;
        case '0':
            fingerMovie.firstFrame();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	if(!frameByframe){
//        int width = ofGetWidth();
//        float pct = (float)x / (float)width;
//        float speed = (2 * pct - 1) * 10.0f;
//        fingerMovie.setSpeed(speed);
//	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
//	if(!frameByframe){
//        int width = ofGetWidth();
//        float pct = (float)x / (float)width;
//        fingerMovie.setPosition(pct);
//	}
    if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        float speed = (2 * pct - 1) * 5.0f;
        fingerMovie.setSpeed(speed);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
//        fingerMovie.setPaused(true);
	}
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
//        fingerMovie.setPaused(false);
        fingerMovie.setSpeed(1.0);
        fingerMovie.setPosition(alteredMovie.getPosition());
        
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
