/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MPEVstProjectAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Timer
{
public:
    MPEVstProjectAudioProcessorEditor (MPEVstProjectAudioProcessor&);
    ~MPEVstProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void setEffectSliderVisibility(int mode);
    void setTuning(int tuning);
    void enableNotes(bool notesEnabled[]);
    void setNoteCents(int noteCents[]);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MPEVstProjectAudioProcessor& audioProcessor;

    //====================================== MASTER VOULUME SLIDER ===============================================================
    juce::Slider masterSlider;
    juce::Label masterLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterSliderAttachment;

    //====================================== VOICE ADSR SLIDERS ===============================================================
    juce::Slider voiceAttackSlider;
    juce::Label voiceAttackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceAttackSliderAttachment;

    juce::Slider voiceDecaySlider;
    juce::Label voiceDecayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceDecaySliderAttachment;

    juce::Slider voiceSustainSlider;
    juce::Label voiceSustainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceSustainSliderAttachment;

    juce::Slider voiceReleaseSlider;
    juce::Label voiceReleaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voiceReleaseSliderAttachment;

    //====================================== OSC 1 UI OBJECTS ===============================================================
    juce::ComboBox osc1Choice;
    juce::Label osc1ChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1ChoiceAttachment;

    juce::Slider osc1LevelSlider;
    juce::Label osc1LevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1LevelSliderAttachment;

    juce::Slider osc1SemitonesSlider;
    juce::Label osc1SemitonesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1SemitonesSliderAttachment;

    juce::Slider osc1DetuneSlider;
    juce::Label osc1DetuneLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc1DetuneSliderAttachment;

    //====================================== OSC 2 UI OBJECTS ===============================================================
    juce::ComboBox osc2Choice;
    juce::Label osc2ChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2ChoiceAttachment;

    juce::Slider osc2LevelSlider;
    juce::Label osc2LevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2LevelSliderAttachment;

    juce::Slider osc2SemitonesSlider;
    juce::Label osc2SemitonesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2SemitonesSliderAttachment;

    juce::Slider osc2DetuneSlider;
    juce::Label osc2DetuneLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc2DetuneSliderAttachment;

    //====================================== OSC 3 UI OBJECTS ===============================================================
    juce::ComboBox osc3Choice;
    juce::Label osc3ChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc3ChoiceAttachment;

    juce::Slider osc3LevelSlider;
    juce::Label osc3LevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc3LevelSliderAttachment;

    juce::Slider osc3SemitonesSlider;
    juce::Label osc3SemitonesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc3SemitonesSliderAttachment;

    juce::Slider osc3DetuneSlider;
    juce::Label osc3DetuneLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> osc3DetuneSliderAttachment;

    //====================================== NOISE OSC SLIDER ===============================================================
    juce::Slider noiseLevelSlider;
    juce::Label noiseLevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseLevelSliderAttachment;

    //====================================== NUMBER OF VOICES SLIDER ===============================================================
    juce::ToggleButton monophonic;
    juce::Label monophonicLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monophonicAttachment;

    juce::Label numOfActiveVoices;

    //====================================== FILTER UI OBJECTS ===============================================================
    juce::Slider filterCutoffSlider;
    juce::Label filterCutoffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffSliderAttachment;

    juce::Slider filterResonanceSlider;
    juce::Label filterResonanceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceSliderAttachment;

    juce::ComboBox filterTypeChoice;
    juce::Label filterTypeChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeChoiceAttachment;

    juce::ComboBox filterModeChoice;
    juce::Label filterModeChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterModeChoiceAttachment;
    
    //====================================== FILTER ADSR SLIDERS ===============================================================
    juce::Slider filterAttackSlider;
    juce::Label filterAttackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackSliderAttachment;

    juce::Slider filterDecaySlider;
    juce::Label filterDecayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecaySliderAttachment;

    juce::Slider filterSustainSlider;
    juce::Label filterSustainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainSliderAttachment;

    juce::Slider filterReleaseSlider;
    juce::Label filterReleaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseSliderAttachment;

    juce::Slider filterADSRAmplitudeSlider;
    juce::Label filterADSRAmplitudeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterADSRAmplitudeSliderAttachment;

    //====================================== FILTER LFO UI OBJECTS ===============================================================
    juce::ComboBox filterLFOOscChoice;
    juce::Label filterLFOOscChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterLFOOscChoiceAttachment;

    juce::Slider filterLFORateSlider;
    juce::Label filterLFORateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterLFORateSliderAttachment;

    juce::Slider filterLFODepthSlider;
    juce::Label filterLFODepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterLFODepthSliderAttachment;

    //====================================== EFFECT CHOICE COMBOBOX ===============================================================
    juce::ComboBox effectChoice;
    juce::Label effectChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> effectChoiceAttachment;

    //====================================== CHORUS SLIDERS ===============================================================
    juce::Slider chorusRateSlider;
    juce::Label chorusRateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusRateSliderAttachment;

    juce::Slider chorusDepthSlider;
    juce::Label chorusDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusDepthSliderAttachment;

    juce::Slider chorusMixSlider;
    juce::Label chorusMixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusMixSliderAttachment;

    juce::Slider chorusFeedbackSlider;
    juce::Label chorusFeedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusFeedbackSliderAttachment;

    juce::Slider chorusCentreDelaySlider;
    juce::Label chorusCentreDelayLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusCentreDelaySliderAttachment;

    //====================================== PHASER SLIDERS ===============================================================
    juce::Slider phaserRateSlider;
    juce::Label phaserRateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserRateSliderAttachment;

    juce::Slider phaserDepthSlider;
    juce::Label phaserDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserDepthSliderAttachment;

    juce::Slider phaserMixSlider;
    juce::Label phaserMixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserMixSliderAttachment;

    juce::Slider phaserFeedbackSlider;
    juce::Label phaserFeedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserFeedbackSliderAttachment;

    juce::Slider phaserCentreFrequencySlider;
    juce::Label phaserCentreFrequencyLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserCentreFrequencySliderAttachment;

    //====================================== REVERB SLIDERS ===============================================================
    juce::Slider reverbRoomSizeSlider;
    juce::Label reverbRoomSizeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbRoomSizeSliderAttachment;

    juce::Slider reverbWidthSlider;
    juce::Label reverbWidthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWidthSliderAttachment;

    juce::Slider reverbDampingSlider;
    juce::Label reverbDampingLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDampingSliderAttachment;

    juce::Slider reverbWetSlider;
    juce::Label reverbWetLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWetSliderAttachment;

    juce::Slider reverbDrySlider;
    juce::Label reverbDryLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDrySliderAttachment;

    //====================================== TUNING ROOT NOTE CHOICE COMBOBOX ===============================================================
    juce::ComboBox rootNoteChoice;
    juce::Label rootNoteChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> rootNoteChoiceAttachment;

    //====================================== TUNING CHOICE COMBOBOX ===============================================================
    juce::ComboBox tuningChoice;
    juce::Label tuningChoiceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> tuningChoiceAttachment;

    //====================================== INTERVAL CENTS SLIDERS ====================================================
    juce::Slider rootSlider;
    juce::Label rootLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rootSliderAttachment;

    juce::Slider minorSecondSlider;
    juce::Label minorSecondLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minorSecondSliderAttachment;

    juce::Slider majorSecondSlider;
    juce::Label majorSecondLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> majorSecondSliderAttachment;

    juce::Slider minorThirdSlider;
    juce::Label minorThirdLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minorThirdSliderAttachment;

    juce::Slider majorThirdSlider;
    juce::Label majorThirdLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> majorThirdSliderAttachment;

    juce::Slider perfectFourthSlider;
    juce::Label perfectFourthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> perfectFourthSliderAttachment;

    juce::Slider diminishedFifthSlider;
    juce::Label diminishedFifthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> diminishedFifthSliderAttachment;

    juce::Slider perfectFifthSlider;
    juce::Label perfectFifthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> perfectFifthSliderAttachment;

    juce::Slider minorSixthSlider;
    juce::Label minorSixthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minorSixthSliderAttachment;

    juce::Slider majorSixthSlider;
    juce::Label majorSixthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> majorSixthSliderAttachment;

    juce::Slider minorSeventhSlider;
    juce::Label minorSeventhLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minorSeventhSliderAttachment;

    juce::Slider majorSeventhSlider;
    juce::Label majorSeventhLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> majorSeventhSliderAttachment;

    //====================================== INTERVAL ENABLE TICKBOXES ====================================================
    juce::ToggleButton rootIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> rootIsEnabledAttachment;

    juce::ToggleButton minorSecondIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> minorSecondIsEnabledAttachment;

    juce::ToggleButton majorSecondIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> majorSecondIsEnabledAttachment;

    juce::ToggleButton minorThirdIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> minorThirdIsEnabledAttachment;

    juce::ToggleButton majorThirdIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> majorThirdIsEnabledAttachment;

    juce::ToggleButton perfectFourthIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> perfectFourthIsEnabledAttachment;

    juce::ToggleButton diminishedFifthIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> diminishedFifthIsEnabledAttachment;

    juce::ToggleButton perfectFifthIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> perfectFifthIsEnabledAttachment;

    juce::ToggleButton minorSixthIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> minorSixthIsEnabledAttachment;

    juce::ToggleButton majorSixthIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> majorSixthIsEnabledAttachment;

    juce::ToggleButton minorSeventhIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> minorSeventhIsEnabledAttachment;

    juce::ToggleButton majorSeventhIsEnabled;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> majorSeventhIsEnabledAttachment;

    //=============================================================================================================
    
    std::unique_ptr<juce::TooltipWindow> tooltipWindow;


    


    //===============================================================================================================
    juce::LookAndFeel_V4 lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MPEVstProjectAudioProcessorEditor)
};
