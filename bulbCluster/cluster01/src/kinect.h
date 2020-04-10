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
    vector<ofPoint> * getPointCloud();
       
    
private:
    
    ofxKinect kinect;
    
    vector <ofPoint> pointCloud;
        
    int w;
    int h;
    int worldSize;
    

};
