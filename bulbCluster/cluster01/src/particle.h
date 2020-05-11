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
    
    // abrennec:
    // Kannst Du einen reference type nehmen?
        void setAttractor(ofPoint * attractor);

		void reset(const int& containerSize);
		void update(const int& containerSize);
		void draw();
    
    // abrennec:
    // private?
    
        // member variables
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
    // abrennec:
    // Kannst Du einen reference type nehmen?
		vector <ofPoint> * attractPoints;
    
        ofPoint * singleAttractor;
    
};
