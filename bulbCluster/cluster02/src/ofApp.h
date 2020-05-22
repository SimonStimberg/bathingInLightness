#pragma once

#include "ofMain.h"
#include "particle.h"
#include "bulb.h"
#include "kinect.h"
#include "synth.h"



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
		
    // abrennec:
    // private?
    // Du koenntest hier auch aus Sicht der Objektorientierung
    // nochmal abstrahieren und bspw. ein Objekt einführen, das sich
    // quasi um alles, was mit den Bulbs und Clustern zu tun hat,
    // kümmert, und die Funktionalität aus ofApp noch ein wenig
    // auslagern. Dann müsstest Du vlt auch noch etwas mehr Daten
    // hin und her schieben und könntest üben, wie man reference types
    // nutzt ;-) 
    
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

        PolySynth	synth;
    
    
    
        // display flags
        bool drawParticles;
        bool drawBulbs;
        bool drawCam;
        bool drawSynthControls;
    
    
    
		
};
