#pragma once

#include "ofMain.h"
#include "particle.h"
#include "bulb.h"
//#include "objDetector.h"
#include "kinect.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void createRandomBulbCluster();
        void initPhysicalCluster();
        void updateAttractor();
    
        void sendToArduino(vector <float> &intensities);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		particleMode currentMode;
    
        int boxSize;

		vector <particle> p;
    
        ofPoint detectionPoint;
        ofPoint singleAttractor;
        bool attractorFound;
        
        vector <bulb> bulbCluster;

    
        // serial communication
        ofSerial serial;
        int timeCheck;
        int refreshRate;
    
    
        // object recognition
//        objDetector detector;
        int lastDetection;
        bool bLearnBakground;
        unsigned int nextJump;
    
        kinectHandler kinectToPoints;
    
    
    
        // display flags
        bool drawParticles;
        bool drawBulbs;
        bool drawCam;
    
    
    
		
};
