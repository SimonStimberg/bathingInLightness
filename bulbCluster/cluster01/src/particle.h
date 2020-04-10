#pragma once
#include "ofMain.h"

enum particleMode{
    NEGATIVE = 0,
    NEUTRAL,
    POSITIVE
};

//enum moodSwitch{
//    NEGATIVE = 0,
//    NEUTRAL,
//    POSITIVE
//};

class particle{

	public:
        particle();
		particle(int bSize); // default constructor
		
        // member functions
		void setMode(particleMode newMode);	
        void setAttractor(ofPoint * attractor);

		void reset(const int& containerSize);
		void update(const int& containerSize);
		void draw();
    
    
        // member variables
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector <ofPoint> * attractPoints;
    
        ofPoint * singleAttractor;
    
};