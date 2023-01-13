/*
  ==============================================================================

    MPEAudioEngine.h
    Created: 12 Dec 2020 3:27:13am
    Author:  Sisma

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "MPEVoice.h"

class MPEAudioEngine : public juce::MPESynthesiser


{
public:
 
    //==============================================================================
    MPEAudioEngine()
    {
       
    }
    
    //==============================================================================
    void prepare(const juce::dsp::ProcessSpec& spec,juce::AudioProcessorValueTreeState *apvts) noexcept
    {
        setCurrentPlaybackSampleRate(spec.sampleRate);        

        //========================= PREPARING VOICES ==================================================================
        monophonic = (bool)apvts->getRawParameterValue("MONOPHONIC")->load();

        if (monophonic) {
            maxNumVoicesSoftCap = 1;
        }
        else {
            maxNumVoicesSoftCap = 8;
        }
        clearVoices();

        for (auto i = 0; i < 2 * maxNumVoicesSoftCap; ++i)
            addVoice(new MPEVoice);
        

        setVoiceStealingEnabled(true);

        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->prepare(spec, apvts);

        //reduceNumVoices(2 * maxNumVoicesSoftCap);




        //========================= PREPARING CHORUS ==================================================================
        chorus.prepare(spec);

        chorus.setCentreDelay(apvts->getRawParameterValue("CHORUS_CENTRE_DELAY")->load());
        chorus.setDepth(apvts->getRawParameterValue("CHORUS_DEPTH")->load());
        chorus.setFeedback(apvts->getRawParameterValue("CHORUS_FEEDBACK")->load());
        chorus.setRate(apvts->getRawParameterValue("CHORUS_RATE")->load());
        chorus.setMix(apvts->getRawParameterValue("CHORUS_MIX")->load());
        
        //========================= PREPARING PHASER ==================================================================     
        phaser.prepare(spec);

        phaser.setCentreFrequency(apvts->getRawParameterValue("PHASER_CENTRE_FREQUENCY")->load());
        phaser.setDepth(apvts->getRawParameterValue("PHASER_DEPTH")->load());
        phaser.setRate(apvts->getRawParameterValue("PHASER_RATE")->load());
        phaser.setFeedback(apvts->getRawParameterValue("PHASER_FEEDBACK")->load());
        phaser.setMix(apvts->getRawParameterValue("PHASER_MIX")->load());

        //========================= PREPARING REVERB ==================================================================
        reverb.prepare(spec);

        reverbParams.damping = apvts->getRawParameterValue("REVERB_DAMPING")->load();
        reverbParams.roomSize = apvts->getRawParameterValue("REVERB_ROOM_SIZE")->load();
        reverbParams.width = apvts->getRawParameterValue("REVERB_WIDTH")->load();
        reverbParams.wetLevel = apvts->getRawParameterValue("REVERB_WET")->load();
        reverbParams.dryLevel = apvts->getRawParameterValue("REVERB_DRY")->load();

        reverb.setParameters(reverbParams);      
    }

    //=============================================================================
    void handleMidiEvent(const juce::MidiMessage& m) override
    {
        if (m.isNoteOn())
        {
            for (auto* v : voices)
            {
                if (m.getNoteNumber() == v->getCurrentlyPlayingNote().initialNote) {
                    dynamic_cast<MPEVoice*>(v)->noteForceStopped();
                }                
            }
        }

        if (m.isController())
            handleController(m.getChannel(), m.getControllerNumber(), m.getControllerValue());
        else if (m.isProgramChange())
            handleProgramChange(m.getChannel(), m.getProgramChangeNumber());

        MPESynthesiserBase::handleMidiEvent(m);

    }

    //================================================================================
    int getNumActiveVoices() {
        int count = 0;
        for (auto* v : voices) {
            if (v->isActive()) {
                count++;
            }
        }
        return count;
    }

//====================================== PARAMETER SETTING ============================================================================================================

    //====================== ADSR SETTINGS ===================================================
    void setVoiceAttack(float attackValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setVoiceAttack(attackValue);
    }
    void setVoiceDecay(float decayValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setVoiceDecay(decayValue);
    }
    void setVoiceSustain(float sustainValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setVoiceSustain(sustainValue);
    }
    void setVoiceRelease(float releaseValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setVoiceRelease(releaseValue);
    }
    //====================== OSC 1 SETTINGS ===================================================
    void setOsc1(int waveformChoice) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc1(waveformChoice);
    }
    void setOsc1Level(float oscLevel) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc1Level(oscLevel);
    }
    void setOsc1Semitones(int oscSemitones) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc1Semitones(oscSemitones);
    }
    void setOsc1Detune(float oscDetune) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc1Detune(oscDetune);
    }
    //====================== OSC 2 SETTINGS ===================================================
    void setOsc2(int waveformChoice) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc2(waveformChoice);
    }
    void setOsc2Level(float oscLevel) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc2Level(oscLevel);
    }
    void setOsc2Semitones(int oscSemitones) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc2Semitones(oscSemitones);
    }
    void setOsc2Detune(float oscDetune) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc2Detune(oscDetune);
    }
    //====================== OSC 3 SETTINGS ===================================================
    void setOsc3(int waveformChoice) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc3(waveformChoice);
    }
    void setOsc3Level(float oscLevel) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc3Level(oscLevel);
    }
    void setOsc3Semitones(int oscSemitones) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc3Semitones(oscSemitones);
    }
    void setOsc3Detune(float oscDetune) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setOsc3Detune(oscDetune);
    }
    //====================== NOISE OSC SETTINGS ===================================================
    void setNoiseLevel(float noiseLevel) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setNoiseLevel(noiseLevel);
    }
    //====================== NUMBER OF VOICES SETTINGS ===================================================
    void setNumOfVoices(bool newMonophonic, const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState* apvts) {
        
        int newNumOfVoices;
        if (newMonophonic)
        {
            newNumOfVoices = 1;
        }
        else
        {
            newNumOfVoices = 8;
        }

        if (newNumOfVoices < maxNumVoicesSoftCap)
        {
            reduceNumVoices(2*newNumOfVoices);
        }
        else
        {
            int voicesToAdd = 2*(newNumOfVoices - maxNumVoicesSoftCap);
            for (int i = 0; i < voicesToAdd; i++)
                addVoice(new MPEVoice);
            for (auto* v : voices)
                dynamic_cast<MPEVoice*> (v)->prepare(spec, apvts);
        }
        maxNumVoicesSoftCap = newNumOfVoices;
        
    }
    //====================== FILTER SETTINGS ===================================================
    void setFilterCutoff(float cutoff) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterCutoff(cutoff);
    }
    void setFilterResonance(float resonance) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterResonance(resonance);
    }
    void setFilterType(int filterTypeChoice) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterType(filterTypeChoice);
    }
    void setFilterMode(int filterMode) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterMode(filterMode);
    }
    //====================== FILTER ADSR SETTINGS ===================================================
    void setFilterAttack(float attackValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterAttack(attackValue);
    }
    void setFilterDecay(float decayValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterDecay(decayValue);
    }
    void setFilterSustain(float sustainValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterSustain(sustainValue);
    }
    void setFilterRelease(float releaseValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterRelease(releaseValue);
    }
    void setFilterADSRAmplitude(float amplitudeValue) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterADSRAmplitude(amplitudeValue);
    }
    //====================== FILTER LFO SETTINGS ===================================================
    void setFilterLFOOsc(int oscChoice) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterLFOOsc(oscChoice);
    }
    void setFilterLFORate(float rate) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterLFORate(rate);
    }
    void setFilterLFODepth(float depth) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setFilterLFODepth(depth);
    }
    //====================== CHORUS SETTINGS ===================================================
    void setChorusRate(float rate) {
        chorus.setRate(rate);
    }
    void setChorusDepth(float depth) {
        chorus.setDepth(depth);
    }
    void setChorusFeedback(float feedback) {
        chorus.setFeedback(feedback);
    }
    void setChorusCentreDelay(float delay) {
        chorus.setCentreDelay(delay);
    }
    void setChorusMix(float mix) {
        chorus.setMix(mix);
    }
    //====================== PHASER SETTINGS ===================================================
    void setPhaserRate(float rate) {
        phaser.setRate(rate);
    }
    void setPhaserDepth(float depth) {
        phaser.setDepth(depth);
    }
    void setPhaserFeedback(float feedback) {
        phaser.setFeedback(feedback);
    }
    void setPhaserCentreFrequency(float freq) {
        phaser.setCentreFrequency(freq);
    }
    void setPhaserMix(float mix) {
        phaser.setMix(mix);
    }
    //====================== REVERB SETTINGS ===================================================
    void setReverbRoomSize(float roomSize) {
        reverbParams.roomSize = roomSize;
        reverb.setParameters(reverbParams);
    }
    void setReverbWidth(float width) {
        reverbParams.width = width;
        reverb.setParameters(reverbParams);
    }
    void setReverbDamping(float damping) {
        reverbParams.damping = damping;
        reverb.setParameters(reverbParams);
    }
    void setReverbWet(float wet) {
        reverbParams.wetLevel = wet;
        reverb.setParameters(reverbParams);
    }
    void setReverbDry(float dry) {
        reverbParams.dryLevel = dry;
        reverb.setParameters(reverbParams);
    }
    //====================== TUNING SETTINGS ===================================================
    void setRootNoteChoice(int rootNote) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setRootNoteChoice(rootNote);
    }

    void setRootCents(int newRootCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setRootCents(newRootCents);
    }
    void setMinorSecondCents(int newMinorSecondCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSecondCents(newMinorSecondCents);
    }
    void setMajorSecondCents(int newMajorSecondCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSecondCents(newMajorSecondCents);
    }
    void setMinorThirdCents(int newMinorThirdCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorThirdCents(newMinorThirdCents);
    }
    void setMajorThirdCents(int newMajorThirdCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorThirdCents(newMajorThirdCents);
    }
    void setPerfectFourthCents(int newPerfectFourthCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setPerfectFourthCents(newPerfectFourthCents);
    }
    void setDiminishedFifthCents(int newDiminishedFifthCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setDiminishedFifthCents(newDiminishedFifthCents);
    }
    void setPerfectFifthCents(int newPerfectFifthCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setPerfectFifthCents(newPerfectFifthCents);
    }
    void setMinorSixthCents(int newMinorSixthCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSixthCents(newMinorSixthCents);
    }
    void setMajorSixthCents(int newMajorSixthCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSixthCents(newMajorSixthCents);
    }
    void setMinorSeventhCents(int newMinorSeventhCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSeventhCents(newMinorSeventhCents);
    }
    void setMajorSeventhCents(int newMajorSeventhCents) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSeventhCents(newMajorSeventhCents);
    }

    void setRootIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setRootIsEnabled(isEnabled);
    }
    void setMinorSecondIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSecondIsEnabled(isEnabled);
    }
    void setMajorSecondIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSecondIsEnabled(isEnabled);
    }
    void setMinorThirdIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorThirdIsEnabled(isEnabled);
    }
    void setMajorThirdIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorThirdIsEnabled(isEnabled);
    }
    void setPerfectFourthIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setPerfectFourthIsEnabled(isEnabled);
    }
    void setDiminishedFifthIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setDiminishedFifthIsEnabled(isEnabled);
    }
    void setPerfectFifthIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setPerfectFifthIsEnabled(isEnabled);
    }
    void setMinorSixthIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSixthIsEnabled(isEnabled);
    }
    void setMajorSixthIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSixthIsEnabled(isEnabled);
    }   
    void setMinorSeventhIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMinorSeventhIsEnabled(isEnabled);
    }
    void setMajorSeventhIsEnabled(bool isEnabled) {
        for (auto* v : voices)
            dynamic_cast<MPEVoice*> (v)->setMajorSeventhIsEnabled(isEnabled);
    }
 
    //==============================================================================

private:
    //==============================================================================
    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {

        MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);
        
        auto block = juce::dsp::AudioBlock<float>(outputAudio);
        auto blockToUse = block.getSubBlock((size_t)startSample, (size_t)numSamples);
        auto contextToUse = juce::dsp::ProcessContextReplacing<float>(blockToUse);
        
    
        chorus.process(contextToUse);
        phaser.process(contextToUse);      
        reverb.process(contextToUse);

        if (getNumActiveVoices() > maxNumVoicesSoftCap) {
            for(int i = 0; i < getNumActiveVoices()-maxNumVoicesSoftCap;++i)
                killOldestVoice();
        }
    }

    //==============================================================================
    void killOldestVoice() {

        juce::MPESynthesiserVoice* voiceToStop = voices[0];
        for (auto* v : voices) {        
            if (voiceToStop->noteOnTime > v->noteOnTime && v->isActive())
                voiceToStop = v;
        }
        
        dynamic_cast<MPEVoice*>(voiceToStop)->noteForceStopped();       
    }

    //==============================================================================
    bool monophonic;
    int maxNumVoicesSoftCap;

    ////============================ EFFECT RELATED VARIABLES ==================================================
    juce::dsp::Chorus<float> chorus;
    juce::dsp::Phaser<float> phaser;
    juce::dsp::Reverb reverb;
    juce::Reverb::Parameters reverbParams;
    
};
