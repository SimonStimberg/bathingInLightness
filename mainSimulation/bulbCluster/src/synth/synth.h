
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synthFX.h"

class PolySynth : public pdsp::Patchable {

public:


    class Voice : public pdsp::Patchable { // internal class ----------------------
        //friend class PolySynth;
    public:
        Voice(){}
        Voice(const Voice& other){}
        
        void setup(PolySynth & ui, int v, int pitch);  
            
        pdsp::Patchable & pan_front() { return in("panFront"); }
        pdsp::Patchable & pan_back() { return in("panBack"); }
        pdsp::Patchable & pan_depth() { return in("panDepth"); }
        
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
        pdsp::LFO					vibratoLfo;

        pdsp::VAFilter              filter;
        pdsp::LFO			        filterModLfo;
        pdsp::Amp                   voiceAmp;
        pdsp::LinearCrossfader      mix;

        pdsp::Panner                panDepth;
        pdsp::Panner                panFront;
        pdsp::Panner                panBack;
        pdsp::Parameter             panValue;
        
        // pdsp::ADSR          ampEnv;
        // pdsp::ADSR          modEnv;
        
        pdsp::LFO           lfo;
     
    }; // end Voice class -------------------------------------------------------




    // public API --------------------------------------------------------------
    void setup( vector <int> pitches );
    void patch();
    void setUI();
    void setVoiceLevels(vector <float> & levels);
    void setVoicePans(vector <glm::vec2> & panPos);


    // pdsp modules
    pdsp::Engine            engine;
	pdsp::ParameterGain     gain;
    pdsp::ParameterGain     masterBus;

    pdsp::LowCut            loCutF;
    pdsp::LowCut            loCutB;
    pdsp::Compressor        compF;
    pdsp::Compressor        compB;
    StereoDelay             delayF;
    StereoDelay             delayB;
    StereoChorus            chorusF;
    StereoChorus            chorusB;
    pdsp::Saturator1        overdriveFL;
    pdsp::Saturator1        overdriveFR;
    pdsp::Saturator1        overdriveBL;
    pdsp::Saturator1        overdriveBR;

    std::vector<Voice>  voices;
    std::vector<pdsp::Parameter>    voiceLevels;
    std::vector<pdsp::Parameter>    voiceFilterCutoffs;




    // GUI stuff
    ofxPanel            gui;
    ofParameterGroup    uiOsc;
        ofParameterGroup    osc1;
        ofParameterGroup    osc2;
    ofParameterGroup    uiFilter;
    
    ofxPanel            guiVoices;
   


private: // --------------------------------------------------------------------
    
    // oscillators control
    pdsp::ParameterAmp  drift_amt;
    pdsp::Parameter     oscMix;

    pdsp::Parameter     pw;
    pdsp::Parameter     pwmSpeed;
    pdsp::ParameterAmp  pwmAmt;  

    pdsp::Parameter     octaveShift;
    pdsp::Parameter     fineTune;

    pdsp::Parameter     vibratoSpeed;
    pdsp::ParameterAmp  vibratoAmt;


    // filter control
    pdsp::Parameter     cutoff;
    pdsp::Parameter     reso;		
    
    pdsp::Parameter     filterModSpeed;
	pdsp::ParameterAmp  filterModAmt; 

    	

};



