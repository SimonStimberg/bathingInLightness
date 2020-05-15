#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    // the number of pitches defines the number of voices

    // vector <int> pitches = {60, 63, 67, 70, 74} ;
    // vector <int> pitches = {24, 36, 39, 43, 46, 48, 51, 55, 58, 60, 63, 67, 70, 74, 79, 84, 87, 91, 94, 98, 103, 108, 111, 115, 118, 122, 24, 36, 39, 43, 46, 48, 51, 55, 58, 60, 63, 67, 70, 74, 79, 84, 87, 91, 94, 98, 103, 108, 111, 115, 118, 122, 12, 24} ;
    // vector <int> pitches = {36, 43, 48, 55, 60, 63, 67, 70, 74, 79, 84, 87, 91, 94, 98} ;

    vector <int> pitches;
    vector <int> scale = {36, 43, 48, 55, 60, 63, 67, 70, 74, 79, 84, 87, 91, 94, 98, 48, 60, 67, 72, 96} ;
    for(int i = 0; i < 52; i++) {
        pitches.push_back(scale[(int) ofRandom(0,scale.size())]);
    }

    synth.setup( pitches );
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    for( size_t i = 0; i < synth.voiceLevels.size(); ++i ){
        float unit = ofGetWidth() / synth.voiceLevels.size();

        float min = unit * (i);
        float max = unit * (i + 16);

        float lvl = 0;

        if (ofGetMouseX() < min + (max - min) * 0.5) {
            lvl = ofClamp(ofMap(ofGetMouseX(), min, min + (max - min) * 0.5, 0.0f, 1.0f), 0.0f, 1.0f);
        } else {
            lvl = ofClamp(ofMap(ofGetMouseX(), min + (max - min) * 0.5, max, 1.0f, 0.0f), 0.0f, 1.0f);
        }     

        synth.voiceLevels[i].set(lvl);
    }  

    
    synth.gui.draw();
    synth.guiVoices.draw();
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
