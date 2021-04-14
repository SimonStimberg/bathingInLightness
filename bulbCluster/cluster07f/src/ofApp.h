#pragma once

#include "ofMain.h"
#include "bulbs/bulbCluster.h"
#include "kinect.h"
#include "synth/synth.h"
#include "boids/Flock3d.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void createRandomBulbCluster(int bulbNum);
        void initPhysicalCluster();
        void initSynth();
    
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
		


    private:
    
        ofVec3f worldSize;
      
        bulbCluster bulbCluster;
        Flock3d flock;
        kinectHandler kinectToPoints;
        PolySynth	synth;

        float maxIntensityKinect;
        float maxIntensityFlock;
        
    
        // serial communication
        ofSerial serial;
        int timeCheck;
        int refreshRate;
    
    
        // display flags
        bool drawBox;
        bool drawParticles;
        bool drawBulbs;
        bool drawCam;
        bool drawSynthControls;
        bool drawInstructions;
        bool drawKinect;
        bool kinectLEDon;
        bool testSwitch;
    
    	
};
