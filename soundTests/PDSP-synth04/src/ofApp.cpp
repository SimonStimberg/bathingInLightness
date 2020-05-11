#include "ofApp.h"

// modules channels API example
//
// whatever module whic can have more than one input or output channel
// for example stereo modules, or multichannel modules
// let you access the channels with the ch() method
//
// multichannel modules allocate resources for new channels when required
// stereo module usually have a fixed number of channels ( two ).
// check for module reference of the ch() method for knowing what is possible
//
// in this example a single pdsp::ParameterAmp with multiple channels
// controls the amount of individual LFOs going to individual oscillators
// and a single pdsp::VAFilter is used to filter the voices panned in stereo

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    // the number of pitches defines the number of voices
    vector <int> pitches = {60, 63, 67, 70, 74} ;
    
    synth.setup( pitches );
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    synth.gui.draw();
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
