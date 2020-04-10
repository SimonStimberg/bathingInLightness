
#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxMioFlowGLSL.h"
#include "ofxFlowTools.h"

#include "synth/ofSynth.h"

using namespace flowTools;

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();
    
    void audioOut(ofSoundBuffer &outBuffer);
	
	ofVideoGrabber cam;
	ofxCv::FlowFarneback flow;
	ofMesh mesh;

    int stepSize, xSteps, ySteps;
    
    vector<ofVec2f> flowAmount;
    ofxCvGrayscaleImage grayImg;
    
    ofxKinect kinect;
    ofxMioFlowGLSL mioFlow;
    ofFloatPixels flowPix; // contains flow information in pixels

    ftOpticalFlow flowtoolsFlow;
    ofFbo                cameraFbo;
    ofFbo                finalFlow;
    vector< ftFlow* >        flows;
    
    
    
private:
    // synth stuff
    ofSynth synth{8};
    int oscNums;

    ofSoundStream soundStream{};
    ofSoundBuffer lastBuffer{};
    std::mutex audioMutex{};
    
    void setupAudio();
    void setupOscs();
    
//    float ampAry[4]{0, 0, 0, 0};
    
    
};
