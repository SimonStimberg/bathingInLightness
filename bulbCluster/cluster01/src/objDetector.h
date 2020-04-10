//
//  objDetector.hpp
//  clusterSerial_openCV
//
//  Created by Simon Paul Stimberg on 15.07.19.
//  based loosely on the "opencvExample" from the OF computer vision example folder
//

#include "ofMain.h"
#include "ofxOpenCv.h"


class objDetector{
    
public:
    // constructor
    objDetector();
    
    // member functions
    ofPoint getPosition();
    void draw();
    void learnBG();
    
private:
    ofPoint pos;
    ofVideoGrabber vidGrabber;
    
    ofxCvColorImage	colorImg;
    
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    
    ofxCvContourFinder contourFinder;
    
    int camWidth;
    int camHeight;
    
    int threshold;

};