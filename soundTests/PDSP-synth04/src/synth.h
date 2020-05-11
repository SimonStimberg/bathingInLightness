
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

class PolySynth : public pdsp::Patchable {

public:


    class Voice : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        void setup(PolySynth & ui, int v, int pitch);        
        
//        float meter_mod_env() const;
//        float meter_pitch() const;

    private:
        pdsp::PatchNode     voiceTrigger;
        pdsp::PatchNode     voicePitch;
        pdsp::PatchNode     voiceLevel;
        
        pdsp::VAOscillator          osc1;
        pdsp::VAOscillator          osc2;
        pdsp::LFO					driftLFO1;
        pdsp::LFO					driftLFO2;

        pdsp::VAFilter              filter;
        pdsp::Amp                   voiceAmp;
        pdsp::LinearCrossfader      mix;
        
        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        pdsp::LFO           lfo;
     
    }; // end Voice class -------------------------------------------------------




    // public API --------------------------------------------------------------
    void setup( vector <int> pitches );
    void patch();
    void setUI();


    // pdsp modules
    pdsp::Engine   engine;
	pdsp::ParameterGain  gain;

    std::vector<Voice>  voices;


    // GUI stuff
    ofxPanel            gui;
    ofParameterGroup    ui;
    ofParameterGroup    uiOsc;
    ofParameterGroup    uiVoices;


private: // --------------------------------------------------------------------
    
    // oscillators control
    pdsp::ParameterAmp  drift_amt;
    pdsp::Parameter     oscMix;

    pdsp::Parameter     pw;
    pdsp::Parameter     pwmSpeed;
    pdsp::ParameterAmp  pwmAmt;  


    // filter control
    pdsp::Parameter     cutoff;
    pdsp::Parameter     reso; 
   

    
    std::vector<pdsp::Parameter>    voiceLevels;

};



