//
//  kinect.cpp
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#include "kinect.h"


// constructor
kinectHandler::kinectHandler(){

}

void kinectHandler::setup(int size) {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
        // enable depth->video image calibration
        kinect.setRegistration(false);
        
       kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        // kinect.init(false, false); // disable video image (faster fps)
        
        kinect.open();        // opens first available kinect
  
        ofLogNotice("Kinect Dimensions: " + ofToString(kinect.getWidth()) + "x" + ofToString(kinect.getHeight()));

    ofSetLogLevel(OF_LOG_NOTICE);

    
    // allocate memory for a certain amount of points
    pointCloud.resize(1000);
    
    worldSize = size;
    
    // should be 640x480
    w = kinect.getWidth();
    h = kinect.getHeight();
    
}



void kinectHandler::update() {
    
    kinect.update();
    
    if(kinect.isFrameNew()) {
        
        pointCloud.clear();

        int step = 20;
        int nearClip = 500;
        int farClip = nearClip + 1000;
        float scaleFactor = 0.2;
        
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
                if(kinect.getDistanceAt(x, y) > nearClip && kinect.getDistanceAt(x, y) < farClip ) {
                    
                    ofPoint newPoint = kinect.getWorldCoordinateAt(x, y);
                    
                    newPoint.z -= nearClip;     // substract near Clipping Plane to match box boundaries
                    newPoint.x *= -1;           // mirror on X Axis
                    newPoint.z *= -1;           // mirror on Z Axis
                    newPoint *= scaleFactor;    // resize to fit simulation
                    
                    // shift to match coordinates of other components
                    newPoint.z += worldSize * 0.5;
                    // newPoint.x += worldSize*0.5;
                    // newPoint.y += worldSize*0.5;
                    
                    pointCloud.push_back(newPoint);
    //                ofLogNotice("found point at: " + ofToString(newPoint));
                    
                }
            }
        }
    }
    
    // ofLogNotice("number of points: " + ofToString(pointCloud.size()));

}



void kinectHandler::draw() {
    
    for(unsigned int i = 0; i < pointCloud.size(); i++) {
        // ofSetColor(255,0,0);
        ofDrawSphere(pointCloud[i], 2.0);
    }
    
//    kinect.drawDepth(0, 0);
    
}

void kinectHandler::drawCam(int x, int y) {

    kinect.drawDepth(x + kinect.getWidth() * 0.5, y - kinect.getHeight() * 0.25, -kinect.getWidth() * 0.5, kinect.getHeight() * 0.5);

}



vector<ofPoint> & kinectHandler::getPointCloud() {
    return pointCloud;
}
