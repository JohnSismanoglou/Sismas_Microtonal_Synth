/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MPEAudioEngine.h"

//==============================================================================
/**
*/
class MPEVstProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MPEVstProjectAudioProcessor();
    ~MPEVstProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    int getNumActiveVoices();
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

//====================================== PARAMETER SETTING ============================================================================================================

    //====================== MASTER VOLUME SETTINGS ===================================================
    void setMasterVolume();
    //====================== ADSR SETTINGS ===================================================
    void setVoiceAttack();
    void setVoiceDecay();
    void setVoiceSustain();
    void setVoiceRelease();
    //====================== OSC 1 SETTINGS ===================================================
    void setOsc1();
    void setOsc1Level();
    void setOsc1Semitones();
    void setOsc1Detune();
    //====================== OSC 2 SETTINGS ===================================================
    void setOsc2();
    void setOsc2Level();
    void setOsc2Semitones();
    void setOsc2Detune();
    //====================== OSC 3 SETTINGS ===================================================
    void setOsc3();
    void setOsc3Level();
    void setOsc3Semitones();
    void setOsc3Detune();
    //====================== NOISE OSC SETTINGS ===================================================
    void setNoiseLevel();
    //====================== NUMBER OF VOICES SETTINGS ===================================================
    void setNumOfVoices();
    //====================== FILTER SETTINGS ===================================================
    void setFilterCutoff();
    void setFilterResonance();
    void setFilterType();
    void setFilterMode();
    //====================== FILTER ADSR SETTINGS ===================================================
    void setFilterAttack();
    void setFilterDecay();
    void setFilterSustain();
    void setFilterRelease();
    void setFilterADSRAmplitude();
    //====================== FILTER LFO SETTINGS ===================================================
    void setFilterLFOOsc();
    void setFilterLFORate();
    void setFilterLFODepth();
    //====================== CHORUS SETTINGS ===================================================
    void setChorusRate();
    void setChorusDepth();
    void setChorusFeedback();
    void setChorusCentreDelay();
    void setChorusMix();
    //====================== PHASER SETTINGS ===================================================
    void setPhaserRate();
    void setPhaserDepth();
    void setPhaserFeedback();
    void setPhaserCentreFrequency();
    void setPhaserMix();
    //====================== REVERB SETTINGS ===================================================
    void setReverbRoomSize();
    void setReverbWidth();
    void setReverbDamping();
    void setReverbWet();
    void setReverbDry();
    //====================== TUNING SETTINGS ===================================================
    void setRootNoteChoice();

    void setRootCents();
    void setMinorSecondCents();
    void setMajorSecondCents();
    void setMinorThirdCents();
    void setMajorThirdCents();
    void setPerfectFourthCents();
    void setDiminishedFifthCents();
    void setPerfectFifthCents();
    void setMinorSixthCents();
    void setMajorSixthCents();
    void setMinorSeventhCents();
    void setMajorSeventhCents();

    void setRootIsEnabled();
    void setMinorSecondIsEnabled();
    void setMajorSecondIsEnabled();
    void setMinorThirdIsEnabled();
    void setMajorThirdIsEnabled();
    void setPerfectFourthIsEnabled();
    void setDiminishedFifthIsEnabled();
    void setPerfectFifthIsEnabled();
    void setMinorSixthIsEnabled();
    void setMajorSixthIsEnabled();
    void setMinorSeventhIsEnabled();
    void setMajorSeventhIsEnabled();

private:
    //================================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    MPEAudioEngine mpeAudioEngine;
    
    juce::dsp::Gain<float> masterVolume;

    double sampleRate;
    int samplesPerBlock;

   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MPEVstProjectAudioProcessor)


};
