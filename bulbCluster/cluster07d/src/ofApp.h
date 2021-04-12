#pragma once

#include "ofMain.h"
#include "bulb.h"
#include "kinect.h"
#include "synth.h"
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
		
    // abrennec:
    // private?
    // Du koenntest hier auch aus Sicht der Objektorientierung
    // nochmal abstrahieren und bspw. ein Objekt einführen, das sich
    // quasi um alles, was mit den Bulbs und Clustern zu tun hat,
    // kümmert, und die Funktionalität aus ofApp noch ein wenig
    // auslagern. Dann müsstest Du vlt auch noch etwas mehr Daten
    // hin und her schieben und könntest üben, wie man reference types
    // nutzt ;-) 
    

    
        int boxSize;
      
        vector <bulb> bulbCluster;

        Flock3d flock;

    
        // serial communication
        ofSerial serial;
        int timeCheck;
        int refreshRate;
    
    
        unsigned int nextJump;
    
        kinectHandler kinectToPoints;

        PolySynth	synth;
    
    
    
        // display flags
        bool drawBox;
        bool drawParticles;
        bool drawBulbs;
        bool drawCam;
        bool drawSynthControls;
        bool drawInstructions;
        bool drawKinect;
    
    
    
		
};
