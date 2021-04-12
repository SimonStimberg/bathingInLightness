//
//  kinect.h
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#include "ofMain.h"
#include "ofxKinect.h"

class kinectHandler{
    
public:
    // constructor
    kinectHandler();
    
    
    // member functions
    void setup(int size);
    void update();
    void draw();
    void exit();
    void drawCam(int x, int y);
    vector<ofPoint> & getPointCloud();
    void setTiltAngle(int angle) { kinect.setCameraTiltAngle(angle); }
       
    
private:
    
    ofxKinect kinect;
    
    vector <ofPoint> pointCloud;
        
    int w;
    int h;
    int worldSize;
    int nearClip;
    int clippingDepth;    

};
