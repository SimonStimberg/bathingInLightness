//
//  kinect.h
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#include "ofMain.h"
#include "ofxKinect.h"

// #define SAFETY_MODE


class kinectHandler{
    
public:
    // constructor
    kinectHandler();
    
    
    // member functions
    void setup(ofVec3f size);
    void update();
    void draw();
    void exit();
    void drawCam(int x, int y);
    vector<ofPoint> & getPointCloud();
    void setTiltAngle(int angle) { kinect.setCameraTiltAngle(angle); }
    void switchLEDon(bool switcher);
       
    
private:
    
    ofxKinect kinect;
    
    vector <ofPoint> pointCloud;
        
    int w;
    int h;
    ofVec3f worldSize;
    int nearClip;
    int clippingDepth;    

};
