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
    vector<ofPoint> & getDemoPoints(bool bPlay);
    void offsetDemoPoints(int offset) { demoIterator += offset; };
    void setTiltAngle(int angle) { kinect[0].setCameraTiltAngle(angle); }
    void switchLEDon(bool switcher);
    float getFlow() { return flow; }
       
    
private:

    void computePointCloud();
    void computeFlow();
        
    static const int numKinects = 2;
    ofxKinect kinect[numKinects];
    
    vector <ofPoint> pointCloud;
    vector <float> flowBuffer;
    int bufferHead;
    int bufferFrameLength;
    int bufferStepSize;
    float flow;

        
    int bufferWidth;
    int bufferHeight;
    ofVec3f worldSize;
    int nearClip;
    int clippingDepth;    

    int demoIterator;
    
};
