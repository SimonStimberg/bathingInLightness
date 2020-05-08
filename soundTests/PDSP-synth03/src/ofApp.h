#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "StereoDelay.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
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

        // pdsp modules
        pdsp::Engine   engine;

        std::vector<pdsp::VAOscillator> oscillators;
        std::vector<pdsp::Parameter>    pitches;
        std::vector<pdsp::LFO>          drift_lfo;
        pdsp::ParameterAmp              drift_amt;
        pdsp::VAFilter                  filter;
        pdsp::Parameter                 cutoff_ctrl;
        pdsp::Parameter                 reso_ctrl;

		pdsp::Parameter                 pw_mod;
		pdsp::Parameter     			pwmSpeed;
		pdsp::ParameterAmp  			pwmAmt; 
		pdsp::LFO						PWMlfo;

		pdsp::Parameter     			filterSpeed;
		pdsp::ParameterAmp  			filterAmt; 
		pdsp::LFO						filterLfo;

		pdsp::Parameter     			vibratoSpeed;
		pdsp::ParameterAmp  			vibratoAmt; 
		pdsp::LFO						vibratoLfo;

		// pdsp::Amp           amp;
    	// pdsp::Saturator1    drive; // distort the signal
		// pdsp::ValueControl  amp_ctrl;


		pdsp::DimensionChorus   chorus;       
        
        pdsp::Parameter chorusSpeed;
        pdsp::Parameter chorusDepth;
		ofParameterGroup     chorusUI;
		ofParameterGroup     oscUI;

		StereoDelay delay;



		pdsp::LinearCrossfader mixL;
		pdsp::LinearCrossfader mixR;
		pdsp::Parameter mixAmount;
        
        pdsp::ParameterGain  gain;


        ofxPanel gui; // probably you will need the gui too
};
