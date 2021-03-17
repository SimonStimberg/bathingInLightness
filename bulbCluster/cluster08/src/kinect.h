//
//  kinect.h
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#pragma once

#include "ofMain.h"
#include "ofxNI2.h"
#include "ofxNiTE2.h"
// #include "ofxKinect.h"

class kinectHandler{
    
public:
    // constructor
    kinectHandler();
    
    
    // member functions
    void setup(int size);
    void exit();
    void update();
    void draw();
    void drawCam(int x, int y);
    vector<ofPoint> & getPointCloud();
       
    
private:
    
    ofxNI2::Device device;
    ofxNiTE2::UserTracker tracker;
    ofPixels depthPixels;
    ofTexture depthTexture;
    
//    ofxKinect kinect;
    
    vector <ofPoint> pointCloud;
        
    int w;
    int h;
    int worldSize;
    

};
