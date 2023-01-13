/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MPEVstProjectAudioProcessorEditor::MPEVstProjectAudioProcessorEditor(MPEVstProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(785, 670);



    tooltipWindow = std::make_unique<juce::TooltipWindow>(this, 700);
    tooltipWindow->setOpaque(false);

    //--------------------- MASTER SLIDER ---------------------------------------------------------------------------
    masterSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    masterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, 50);
    masterLabel.setText("Master Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(masterLabel);
    masterSlider.setTextValueSuffix(" Volume");
    addAndMakeVisible(masterSlider);
    masterSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MASTER", masterSlider);
    masterSlider.onValueChange = [this]
    {
        audioProcessor.setMasterVolume();
    };
    //---------------------- VOICE ATTACK SLIDER ---------------------------------------------------------------------------
    voiceAttackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    voiceAttackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    voiceAttackLabel.setText("A", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(voiceAttackLabel);
    voiceAttackSlider.setTextValueSuffix(" Secs");
    voiceAttackSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    voiceAttackSlider.setTooltip("Note's Envelope Attack.");
    addAndMakeVisible(voiceAttackSlider);
    voiceAttackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOICE_ATTACK", voiceAttackSlider);
    voiceAttackSlider.onValueChange = [this]
    {
        audioProcessor.setVoiceAttack();
    };
    //---------------------- VOICE DECAY SLIDER ---------------------------------------------------------------------------
    voiceDecaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    voiceDecaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    voiceDecayLabel.setText("D", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(voiceDecayLabel);
    voiceDecaySlider.setTextValueSuffix(" Secs");
    voiceDecaySlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    voiceDecaySlider.setTooltip("Note's Envelope Decay.");
    addAndMakeVisible(voiceDecaySlider);
    voiceDecaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOICE_DECAY", voiceDecaySlider);
    voiceDecaySlider.onValueChange = [this]
    {
        audioProcessor.setVoiceDecay();
    };
    //---------------------- VOICE SUSTAIN SLIDER ---------------------------------------------------------------------------
    voiceSustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    voiceSustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    voiceSustainLabel.setText("S", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(voiceSustainLabel);
    voiceSustainSlider.setTextValueSuffix(" Volume");
    voiceSustainSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    voiceSustainSlider.setTooltip("Note's Envelope Sustain.");
    addAndMakeVisible(voiceSustainSlider);
    voiceSustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOICE_SUSTAIN", voiceSustainSlider);
    voiceSustainSlider.onValueChange = [this]
    {
        audioProcessor.setVoiceSustain();
    };
    //---------------------- VOICE RELEASE SLIDER ---------------------------------------------------------------------------
    voiceReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    voiceReleaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    voiceReleaseLabel.setText("R", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(voiceReleaseLabel);
    voiceReleaseSlider.setTextValueSuffix(" Secs");
    voiceReleaseSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    voiceReleaseSlider.setTooltip("Note's Envelope Release.");
    addAndMakeVisible(voiceReleaseSlider);
    voiceReleaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOICE_RELEASE", voiceReleaseSlider);
    voiceReleaseSlider.onValueChange = [this]
    {
        audioProcessor.setVoiceRelease();
    };
    //---------------------- OSC 1 COMBO BOX -------------------------------------------------------------------------------------------
    osc1Choice.addItemList(audioProcessor.apvts.getParameter("OSC_1")->getAllValueStrings(), 1);
    osc1ChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC_1", osc1Choice);
    osc1ChoiceLabel.setText("Osc 1", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc1ChoiceLabel);
    addAndMakeVisible(osc1Choice);
    osc1Choice.onChange = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setOsc1();
        audioProcessor.suspendProcessing(false);
    };
    //---------------------- OSC 2 COMBO BOX -------------------------------------------------------------------------------------------
    osc2Choice.addItemList(audioProcessor.apvts.getParameter("OSC_2")->getAllValueStrings(), 1);
    osc2ChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC_2", osc2Choice);
    osc2ChoiceLabel.setText("Osc 2", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc2ChoiceLabel);
    addAndMakeVisible(osc2Choice);
    osc2Choice.onChange = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setOsc2();
        audioProcessor.suspendProcessing(false);
    };
    //---------------------- OSC 3 COMBO BOX -------------------------------------------------------------------------------------------
    osc3Choice.addItemList(audioProcessor.apvts.getParameter("OSC_3")->getAllValueStrings(), 1);
    osc3ChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC_3", osc3Choice);
    osc3ChoiceLabel.setText("Osc 3", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc3ChoiceLabel);
    addAndMakeVisible(osc3Choice);
    osc3Choice.onChange = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setOsc3();
        audioProcessor.suspendProcessing(false);
    };
    //--------------------- OSC 1 LEVEL SLIDER ---------------------------------------------------------------------------
    osc1LevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc1LevelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc1LevelLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc1LevelLabel);
    osc1LevelSlider.setTextValueSuffix(" Volume");
    osc1LevelSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc1LevelSlider);
    osc1LevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_1_LEVEL", osc1LevelSlider);
    osc1LevelSlider.onValueChange = [this]
    {
        audioProcessor.setOsc1Level();
    };
    //--------------------- OSC 2 LEVEL SLIDER ---------------------------------------------------------------------------
    osc2LevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc2LevelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc2LevelLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc2LevelLabel);
    osc2LevelSlider.setTextValueSuffix(" Volume");
    osc2LevelSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc2LevelSlider);
    osc2LevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_2_LEVEL", osc2LevelSlider);
    osc2LevelSlider.onValueChange = [this]
    {
        audioProcessor.setOsc2Level();
    };
    //--------------------- OSC 3 LEVEL SLIDER ---------------------------------------------------------------------------
    osc3LevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc3LevelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc3LevelLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc3LevelLabel);
    osc3LevelSlider.setTextValueSuffix(" Volume");
    osc3LevelSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc3LevelSlider);
    osc3LevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_3_LEVEL", osc3LevelSlider);
    osc3LevelSlider.onValueChange = [this]
    {
        audioProcessor.setOsc3Level();
    };
    //--------------------- NOISE LEVEL SLIDER ---------------------------------------------------------------------------
    noiseLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    noiseLevelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    noiseLevelLabel.setText("Noise", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(noiseLevelLabel);
    noiseLevelSlider.setTextValueSuffix(" Volume");
    noiseLevelSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(noiseLevelSlider);
    noiseLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "NOISE_LEVEL", noiseLevelSlider);
    noiseLevelSlider.onValueChange = [this]
    {
        audioProcessor.setNoiseLevel();
    };
    //--------------------- MONOPHONIC BUTTON ---------------------------------------------------------------------------
    monophonicLabel.setText("Monophonic", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(monophonicLabel);
    addAndMakeVisible(monophonic);
    monophonicAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MONOPHONIC", monophonic);
    monophonic.onClick = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setNumOfVoices();
        audioProcessor.suspendProcessing(false);
    };

    //--------------------- OSC 1 SEMITONES SLIDER ---------------------------------------------------------------------------
    osc1SemitonesSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    osc1SemitonesSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    osc1SemitonesLabel.setText("Sem", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc1SemitonesLabel);
    addAndMakeVisible(osc1SemitonesSlider);
    osc1SemitonesSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_1_SEMITONES", osc1SemitonesSlider);
    osc1SemitonesSlider.onValueChange = [this]
    {
        audioProcessor.setOsc1Semitones();
    };
    //--------------------- OSC 2 SEMITONES SLIDER ---------------------------------------------------------------------------
    osc2SemitonesSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    osc2SemitonesSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    osc2SemitonesLabel.setText("Sem", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc2SemitonesLabel);
    addAndMakeVisible(osc2SemitonesSlider);
    osc2SemitonesSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_2_SEMITONES", osc2SemitonesSlider);
    osc2SemitonesSlider.onValueChange = [this]
    {
        audioProcessor.setOsc2Semitones();
    };
    //--------------------- OSC 3 SEMITONES SLIDER ---------------------------------------------------------------------------
    osc3SemitonesSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    osc3SemitonesSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 45, 20);
    osc3SemitonesLabel.setText("Sem", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc3SemitonesLabel);
    addAndMakeVisible(osc3SemitonesSlider);
    osc3SemitonesSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_3_SEMITONES", osc3SemitonesSlider);
    osc3SemitonesSlider.onValueChange = [this]
    {
        audioProcessor.setOsc3Semitones();
    };
    //--------------------- OSC 1 DETUNE SLIDER ---------------------------------------------------------------------------
    osc1DetuneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc1DetuneSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc1DetuneLabel.setText("Detune", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc1DetuneLabel);
    osc1DetuneSlider.setTextValueSuffix(" Cents");
    osc1DetuneSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc1DetuneSlider);
    osc1DetuneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_1_DETUNE", osc1DetuneSlider);
    osc1DetuneSlider.onValueChange = [this]
    {
        audioProcessor.setOsc1Detune();
    };
    //--------------------- OSC 2 DETUNE SLIDER ---------------------------------------------------------------------------
    osc2DetuneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc2DetuneSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc2DetuneLabel.setText("Detune", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc2DetuneLabel);
    osc2DetuneSlider.setTextValueSuffix(" Cents");
    osc2DetuneSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc2DetuneSlider);
    osc2DetuneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_2_DETUNE", osc2DetuneSlider);
    osc2DetuneSlider.onValueChange = [this]
    {
        audioProcessor.setOsc2Detune();
    };

    //--------------------- OSC 3 DETUNE SLIDER ---------------------------------------------------------------------------
    osc3DetuneSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    osc3DetuneSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 50);
    osc3DetuneLabel.setText("Detune", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(osc3DetuneLabel);
    osc3DetuneSlider.setTextValueSuffix(" Cents");
    osc3DetuneSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(osc3DetuneSlider);
    osc3DetuneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSC_3_DETUNE", osc3DetuneSlider);
    osc3DetuneSlider.onValueChange = [this]
    {
        audioProcessor.setOsc3Detune();
    };

    //--------------------- FILTER CUTOFF SLIDER ---------------------------------------------------------------------------
    filterCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterCutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);
    filterCutoffLabel.setText("Cutoff", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterCutoffLabel);
    filterCutoffSlider.setTextValueSuffix(" Hz");
    filterCutoffSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    filterCutoffSlider.setTooltip("Filter Cutoff \n-In ADSR Filter Mode it works as the envelopes min value. \n-In Keyboard Track Mode the 220Hz mark is placing the cutoff point \n at the frequency of the note you played. \n-In LFO Mode this is the mid value of the oscillation.");
    addAndMakeVisible(filterCutoffSlider);
    filterCutoffSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", filterCutoffSlider);
    filterCutoffSlider.onValueChange = [this]
    {
        audioProcessor.setFilterCutoff();
    };

    //--------------------- FILTER RESONANCE SLIDER ---------------------------------------------------------------------------
    filterResonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterResonanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 100, 20);
    filterResonanceLabel.setText("Resonance", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterResonanceLabel);
    filterResonanceSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(filterResonanceSlider);
    filterResonanceSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RESONANCE", filterResonanceSlider);
    filterResonanceSlider.onValueChange = [this]
    {
        audioProcessor.setFilterResonance();
    };

    //---------------------- FILTER TYPE COMBO BOX -------------------------------------------------------------------------------------------
    filterTypeChoice.addItemList(audioProcessor.apvts.getParameter("FILTER_CHOICE")->getAllValueStrings(), 1);
    filterTypeChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTER_CHOICE", filterTypeChoice);
    filterTypeChoiceLabel.setText("Filter Type", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterTypeChoiceLabel);
    addAndMakeVisible(filterTypeChoice);
    filterTypeChoice.onChange = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setFilterType();
        audioProcessor.suspendProcessing(false);
    };
    //---------------------- FILTER ATTACK SLIDER ---------------------------------------------------------------------------
    filterAttackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    filterAttackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterAttackLabel.setText("A", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterAttackLabel);
    filterAttackSlider.setTextValueSuffix(" Secs");
    filterAttackSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    filterAttackSlider.setTooltip("Filter's Envelope Attack.");
    addAndMakeVisible(filterAttackSlider);
    filterAttackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_ATTACK", filterAttackSlider);
    filterAttackSlider.onValueChange = [this]
    {
        audioProcessor.setFilterAttack();
    };
    //---------------------- FILTER DECAY SLIDER ---------------------------------------------------------------------------
    filterDecaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    filterDecaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterDecayLabel.setText("D", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterDecayLabel);
    filterDecaySlider.setTextValueSuffix(" Secs");
    filterDecaySlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    filterDecaySlider.setTooltip("Filter's Envelope Decay.");
    addAndMakeVisible(filterDecaySlider);
    filterDecaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_DECAY", filterDecaySlider);
    filterDecaySlider.onValueChange = [this]
    {
        audioProcessor.setFilterDecay();
    };
    //---------------------- FILTER SUSTAIN SLIDER ---------------------------------------------------------------------------
    filterSustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    filterSustainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterSustainLabel.setText("S", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterSustainLabel);
    filterSustainSlider.setTextValueSuffix(" Amplitude");
    filterSustainSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    filterSustainSlider.setTooltip("Filter's Envelope Sustain.");
    addAndMakeVisible(filterSustainSlider);
    filterSustainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_SUSTAIN", filterSustainSlider);
    filterSustainSlider.onValueChange = [this]
    {
        audioProcessor.setFilterSustain();
    };
    //---------------------- FILTER RELEASE SLIDER ---------------------------------------------------------------------------
    filterReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    filterReleaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterReleaseLabel.setText("R", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterReleaseLabel);
    filterReleaseSlider.setTextValueSuffix(" Secs");
    filterReleaseSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    filterReleaseSlider.setTooltip("Filter's Envelope Release.");
    addAndMakeVisible(filterReleaseSlider);
    filterReleaseSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_RELEASE", filterReleaseSlider);
    filterReleaseSlider.onValueChange = [this]
    {
        audioProcessor.setFilterRelease();
    };
    //---------------------- FILTER ADSR AMPLITUDE SLIDER ---------------------------------------------------------------------------
    filterADSRAmplitudeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    filterADSRAmplitudeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterADSRAmplitudeLabel.setText("M", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterADSRAmplitudeLabel);
    filterADSRAmplitudeSlider.setTextValueSuffix(" Hz");
    filterADSRAmplitudeSlider.setPopupDisplayEnabled(true, true, nullptr, 1500);
    filterADSRAmplitudeSlider.setTooltip("Filter's Envelope Max Value.");
    addAndMakeVisible(filterADSRAmplitudeSlider);
    filterADSRAmplitudeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_ADSR_AMPLITUDE", filterADSRAmplitudeSlider);
    filterADSRAmplitudeSlider.onValueChange = [this]
    {
        audioProcessor.setFilterADSRAmplitude();
    };

    //---------------------- FILTER LFO OSC COMBO BOX -------------------------------------------------------------------------------------------
    filterLFOOscChoice.addItemList(audioProcessor.apvts.getParameter("FILTER_LFO_WAVEFORM_CHOICE")->getAllValueStrings(), 1);
    filterLFOOscChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTER_LFO_WAVEFORM_CHOICE", filterLFOOscChoice);
    filterLFOOscChoiceLabel.setText("LFO Waveform", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterLFOOscChoiceLabel);
    addAndMakeVisible(filterLFOOscChoice);
    filterLFOOscChoice.onChange = [this]
    {
        audioProcessor.suspendProcessing(true);
        audioProcessor.setFilterLFOOsc();
        audioProcessor.suspendProcessing(false);
    };
    //---------------------- FILTER LFO RATE SLIDER -------------------------------------------------------------------------------------------
    filterLFORateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterLFORateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterLFORateLabel.setText("LFO Rate", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterLFORateLabel);
    filterLFORateSlider.setTextValueSuffix(" Hz");
    filterLFORateSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(filterLFORateSlider);
    filterLFORateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_LFO_RATE", filterLFORateSlider);
    filterLFORateSlider.onValueChange = [this]
    {
        audioProcessor.setFilterLFORate();
    };
    //---------------------- FILTER LFO DEPTH SLIDER -------------------------------------------------------------------------------------------
    filterLFODepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterLFODepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    filterLFODepthLabel.setText("LFO Depth", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterLFODepthLabel);
    filterLFODepthSlider.setTextValueSuffix(" Amplitude");
    filterLFODepthSlider.setPopupDisplayEnabled(true, false, nullptr, 2000);
    addAndMakeVisible(filterLFODepthSlider);
    filterLFODepthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER_LFO_DEPTH", filterLFODepthSlider);
    filterLFODepthSlider.onValueChange = [this]
    {
        audioProcessor.setFilterLFODepth();
    };
    //---------------------- FILTER MODE COMBO BOX -------------------------------------------------------------------------------------------
    filterModeChoice.addItemList(audioProcessor.apvts.getParameter("FILTER_MODE_CHOICE")->getAllValueStrings(), 1);
    filterModeChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTER_MODE_CHOICE", filterModeChoice);
    filterModeChoiceLabel.setText("Filter Mode", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(filterModeChoiceLabel);
    addAndMakeVisible(filterModeChoice);
    filterModeChoice.onChange = [this]
    {
        audioProcessor.setFilterMode();
    };
    //---------------------- CHORUS RATE SLIDER -------------------------------------------------------------------------------------------
    chorusRateSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusRateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    chorusRateLabel.setText("Rate", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(chorusRateLabel);
    chorusRateSlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(chorusRateSlider);
    chorusRateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CHORUS_RATE", chorusRateSlider);
    chorusRateSlider.onValueChange = [this]
    {
        audioProcessor.setChorusRate();
    };
    //---------------------- CHORUS DEPTH SLIDER -------------------------------------------------------------------------------------------
    chorusDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    chorusDepthLabel.setText("Depth", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(chorusDepthLabel);
    chorusDepthSlider.setTextValueSuffix(" Ampltude");
    addAndMakeVisible(chorusDepthSlider);
    chorusDepthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CHORUS_DEPTH", chorusDepthSlider);
    chorusDepthSlider.onValueChange = [this]
    {
        audioProcessor.setChorusDepth();
    };
    //---------------------- CHORUS MIX SLIDER -------------------------------------------------------------------------------------------
    chorusMixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusMixSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    chorusMixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(chorusMixLabel);
    addAndMakeVisible(chorusMixSlider);
    chorusMixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CHORUS_MIX", chorusMixSlider);
    chorusMixSlider.onValueChange = [this]
    {
        audioProcessor.setChorusMix();
    };
    //---------------------- CHORUS FEEDBACK SLIDER -------------------------------------------------------------------------------------------
    chorusFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    chorusFeedbackLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(chorusFeedbackLabel);
    addAndMakeVisible(chorusFeedbackSlider);
    chorusFeedbackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CHORUS_FEEDBACK", chorusFeedbackSlider);
    chorusFeedbackSlider.onValueChange = [this]
    {
        audioProcessor.setChorusFeedback();
    };
    //---------------------- CHORUS CENTRE DELAY SLIDER -------------------------------------------------------------------------------------------
    chorusCentreDelaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    chorusCentreDelaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    chorusCentreDelayLabel.setText("Centre Delay", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(chorusCentreDelayLabel);
    chorusCentreDelaySlider.setTextValueSuffix(" ms");
    addAndMakeVisible(chorusCentreDelaySlider);
    chorusCentreDelaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CHORUS_CENTRE_DELAY", chorusCentreDelaySlider);
    chorusCentreDelaySlider.onValueChange = [this]
    {
        audioProcessor.setChorusCentreDelay();
    };
    //---------------------- PHASER RATE SLIDER -------------------------------------------------------------------------------------------
    phaserRateSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    phaserRateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    phaserRateLabel.setText("Rate", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(phaserRateLabel);
    phaserRateSlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(phaserRateSlider);
    phaserRateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_RATE", phaserRateSlider);
    phaserRateSlider.onValueChange = [this]
    {
        audioProcessor.setPhaserRate();
    };
    //---------------------- PHASER DEPTH SLIDER -------------------------------------------------------------------------------------------
    phaserDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    phaserDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    phaserDepthLabel.setText("Depth", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(phaserDepthLabel);
    phaserDepthSlider.setTextValueSuffix(" Amplitude");
    addAndMakeVisible(phaserDepthSlider);
    phaserDepthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_DEPTH", phaserDepthSlider);
    phaserDepthSlider.onValueChange = [this]
    {
        audioProcessor.setPhaserDepth();
    };
    //---------------------- PHASER FEEDBACK SLIDER -------------------------------------------------------------------------------------------
    phaserFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    phaserFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    phaserFeedbackLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(phaserFeedbackLabel);
    addAndMakeVisible(phaserFeedbackSlider);
    phaserFeedbackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_FEEDBACK", phaserFeedbackSlider);
    phaserFeedbackSlider.onValueChange = [this]
    {
        audioProcessor.setPhaserFeedback();
    };
    //---------------------- PHASER CENTRE FREQUENCY SLIDER -------------------------------------------------------------------------------------------
    phaserCentreFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    phaserCentreFrequencySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    phaserCentreFrequencyLabel.setText("Centre Freq.", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(phaserCentreFrequencyLabel);
    phaserCentreFrequencySlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(phaserCentreFrequencySlider);
    phaserCentreFrequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_CENTRE_FREQUENCY", phaserCentreFrequencySlider);
    phaserCentreFrequencySlider.onValueChange = [this]
    {
        audioProcessor.setPhaserCentreFrequency();
    };
    //---------------------- PHASER MIX SLIDER -------------------------------------------------------------------------------------------
    phaserMixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    phaserMixSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    phaserMixLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(phaserMixLabel);
    addAndMakeVisible(phaserMixSlider);
    phaserMixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PHASER_MIX", phaserMixSlider);
    phaserMixSlider.onValueChange = [this]
    {
        audioProcessor.setPhaserMix();
    };
    //---------------------- REVERB ROOM SIZE SLIDER -------------------------------------------------------------------------------------------
    reverbRoomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbRoomSizeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    reverbRoomSizeLabel.setText("Room Size", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbRoomSizeLabel);
    addAndMakeVisible(reverbRoomSizeSlider);
    reverbRoomSizeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_ROOM_SIZE", reverbRoomSizeSlider);
    reverbRoomSizeSlider.onValueChange = [this]
    {
        audioProcessor.setReverbRoomSize();
    };
    //---------------------- REVERB WIDTH SLIDER -------------------------------------------------------------------------------------------
    reverbWidthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbWidthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    reverbWidthLabel.setText("Width", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbWidthLabel);
    addAndMakeVisible(reverbWidthSlider);
    reverbWidthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_WIDTH", reverbWidthSlider);
    reverbWidthSlider.onValueChange = [this]
    {
        audioProcessor.setReverbWidth();
    };
    //---------------------- REVERB DAMPING SLIDER -------------------------------------------------------------------------------------------
    reverbDampingSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbDampingSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    reverbDampingLabel.setText("Damping", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbDampingLabel);
    addAndMakeVisible(reverbDampingSlider);
    reverbDampingSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_DAMPING", reverbDampingSlider);
    reverbDampingSlider.onValueChange = [this]
    {
        audioProcessor.setReverbDamping();
    };
    //---------------------- REVERB WET SLIDER -------------------------------------------------------------------------------------------
    reverbWetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    reverbWetLabel.setText("Wet", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbWetLabel);
    addAndMakeVisible(reverbWetSlider);
    reverbWetSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_WET", reverbWetSlider);
    reverbWetSlider.onValueChange = [this]
    {
        audioProcessor.setReverbWet();
    };
    //---------------------- REVERB DRY SLIDER -------------------------------------------------------------------------------------------
    reverbDrySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    reverbDrySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 0, 0);
    reverbDryLabel.setText("Dry", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(reverbDryLabel);
    addAndMakeVisible(reverbDrySlider);
    reverbDrySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_DRY", reverbDrySlider);
    reverbDrySlider.onValueChange = [this]
    {
        audioProcessor.setReverbDry();
    };

    //---------------------- EFFECT CHOICE COMBO BOX -------------------------------------------------------------------------------------------
    effectChoice.addItemList(audioProcessor.apvts.getParameter("EFFECT_CHOICE")->getAllValueStrings(), 1);
    effectChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "EFFECT_CHOICE", effectChoice);
    effectChoiceLabel.setText("Effects", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(effectChoiceLabel);
    effectChoice.setTooltip("-Chorus: for a classic chorus sound \nuse a centre delay time at 7-8 ms \n with a low feedback and a low depth,\n for a flanger sound use a lower\n centre delay time and a lot of feedback, \n if the mix is at 1.0 the effect behaves like a vibrato. \n-Phaser: a basic phaser effect.\n-Reverb: a basic reverb effect.");
    addAndMakeVisible(effectChoice);
    effectChoice.onChange = [this]
    {
        setEffectSliderVisibility(effectChoice.getSelectedId() - 1);
    };

    //---------------------- ROOT NOTE CHOICE COMBO BOX -------------------------------------------------------------------------------------------
    rootNoteChoice.addItemList(audioProcessor.apvts.getParameter("ROOT_NOTE_CHOICE")->getAllValueStrings(), 1);
    rootNoteChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "ROOT_NOTE_CHOICE", rootNoteChoice);
    rootNoteChoiceLabel.setText("Root", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(rootNoteChoiceLabel);
    rootNoteChoice.setTooltip("The note that serves \nas the root of the scale.");
    addAndMakeVisible(rootNoteChoice);
    rootNoteChoice.onChange = [this]
    {
        audioProcessor.setRootNoteChoice();
    };
    //---------------------- ROOT SLIDER -------------------------------------------------------------------------------------------
    rootSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    rootSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    rootSlider.setTextValueSuffix(" Cents");
    rootLabel.setText("Root", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(rootLabel);
    rootSlider.setTooltip("This serves as a global fine tuning\n it applies on all the deggres.");
    addAndMakeVisible(rootSlider);
    rootSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ROOT", rootSlider);
    rootSlider.onValueChange = [this]
    {
        audioProcessor.setRootCents();
    };
    //---------------------- MINOR SECOND SLIDER -------------------------------------------------------------------------------------------
    minorSecondSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    minorSecondSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    minorSecondSlider.setTextValueSuffix(" Cents");
    minorSecondLabel.setText("Minor 2nd", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(minorSecondLabel);
    minorSecondSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(minorSecondSlider);
    minorSecondSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MINOR_SECOND", minorSecondSlider);
    minorSecondSlider.onValueChange = [this]
    {
        audioProcessor.setMinorSecondCents();
    };
    //---------------------- MAJOR SECOND SLIDER -------------------------------------------------------------------------------------------
    majorSecondSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    majorSecondSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    majorSecondSlider.setTextValueSuffix(" Cents");
    majorSecondLabel.setText("Major 2nd", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(majorSecondLabel);
    majorSecondSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(majorSecondSlider);
    majorSecondSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MAJOR_SECOND", majorSecondSlider);
    majorSecondSlider.onValueChange = [this]
    {
        audioProcessor.setMajorSecondCents();
    };
    //---------------------- MINOR THIRD SLIDER -------------------------------------------------------------------------------------------
    minorThirdSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    minorThirdSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    minorThirdSlider.setTextValueSuffix(" Cents");
    minorThirdLabel.setText("Minor 3rd", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(minorThirdLabel);
    minorThirdSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(minorThirdSlider);
    minorThirdSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MINOR_THIRD", minorThirdSlider);
    minorThirdSlider.onValueChange = [this]
    {
        audioProcessor.setMinorThirdCents();
    };
    //---------------------- MAJOR THIRD SLIDER -------------------------------------------------------------------------------------------
    majorThirdSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    majorThirdSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    majorThirdSlider.setTextValueSuffix(" Cents");
    majorThirdLabel.setText("Major 3rd", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(majorThirdLabel);
    majorThirdSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(majorThirdSlider);
    majorThirdSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MAJOR_THIRD", majorThirdSlider);
    majorThirdSlider.onValueChange = [this]
    {
        audioProcessor.setMajorThirdCents();
    };
    //---------------------- PERFECT FOURTH SLIDER -------------------------------------------------------------------------------------------
    perfectFourthSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    perfectFourthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    perfectFourthSlider.setTextValueSuffix(" Cents");
    perfectFourthLabel.setText("Perfect 4th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(perfectFourthLabel);
    perfectFourthSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(perfectFourthSlider);
    perfectFourthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PERFECT_FOURTH", perfectFourthSlider);
    perfectFourthSlider.onValueChange = [this]
    {
        audioProcessor.setPerfectFourthCents();
    };
    //---------------------- DIMINISHED FIFTH SLIDER -------------------------------------------------------------------------------------------
    diminishedFifthSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    diminishedFifthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    diminishedFifthSlider.setTextValueSuffix(" Cents");
    diminishedFifthLabel.setText("Dim. 5th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(diminishedFifthLabel);
    diminishedFifthSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(diminishedFifthSlider);
    diminishedFifthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DIMINISHED_FIFTH", diminishedFifthSlider);
    diminishedFifthSlider.onValueChange = [this]
    {
        audioProcessor.setDiminishedFifthCents();
    };
    //---------------------- PERFECT FIFTH SLIDER -------------------------------------------------------------------------------------------
    perfectFifthSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    perfectFifthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    perfectFifthSlider.setTextValueSuffix(" Cents");
    perfectFifthLabel.setText("Perfect 5th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(perfectFifthLabel);
    perfectFifthSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(perfectFifthSlider);
    perfectFifthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PERFECT_FIFTH", perfectFifthSlider);
    perfectFifthSlider.onValueChange = [this]
    {
        audioProcessor.setPerfectFifthCents();
    };
    //---------------------- MINOR SIXTH SLIDER -------------------------------------------------------------------------------------------
    minorSixthSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    minorSixthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    minorSixthSlider.setTextValueSuffix(" Cents");
    minorSixthLabel.setText("Minor 6th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(minorSixthLabel);
    minorSixthSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(minorSixthSlider);
    minorSixthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MINOR_SIXTH", minorSixthSlider);
    minorSixthSlider.onValueChange = [this]
    {
        audioProcessor.setMinorSixthCents();
    };
    //---------------------- MAJOR SIXTH SLIDER -------------------------------------------------------------------------------------------
    majorSixthSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    majorSixthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    majorSixthSlider.setTextValueSuffix(" Cents");
    majorSixthLabel.setText("Major 6th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(majorSixthLabel);
    majorSixthSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(majorSixthSlider);
    majorSixthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MAJOR_SIXTH", majorSixthSlider);
    majorSixthSlider.onValueChange = [this]
    {
        audioProcessor.setMajorSixthCents();
    };
    //---------------------- MINOR SEVENTH SLIDER -------------------------------------------------------------------------------------------
    minorSeventhSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    minorSeventhSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    minorSeventhSlider.setTextValueSuffix(" Cents");
    minorSeventhLabel.setText("Minor 7th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(minorSeventhLabel);
    minorSeventhSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(minorSeventhSlider);
    minorSeventhSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MINOR_SEVENTH", minorSeventhSlider);
    minorSeventhSlider.onValueChange = [this]
    {
        audioProcessor.setMinorSeventhCents();
    };
    //---------------------- MAJOR SEVENTH SLIDER -------------------------------------------------------------------------------------------
    majorSeventhSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    majorSeventhSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 16);
    majorSeventhSlider.setTextValueSuffix(" Cents");
    majorSeventhLabel.setText("Major 7th", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(majorSeventhLabel);
    majorSeventhSlider.setTooltip("Distance of the note from the root.");
    addAndMakeVisible(majorSeventhSlider);
    majorSeventhSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MAJOR_SEVENTH", majorSeventhSlider);
    majorSeventhSlider.onValueChange = [this]
    {
        audioProcessor.setMajorSeventhCents();
    };
    //---------------------- ROOT IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(rootIsEnabled);
    rootIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "ROOT_IS_ENABLED", rootIsEnabled);
    rootIsEnabled.onClick = [this]
    {
        audioProcessor.setRootIsEnabled();
    };
    //---------------------- MINOR SECOND IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(minorSecondIsEnabled);
    minorSecondIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MINOR_SECOND_IS_ENABLED", minorSecondIsEnabled);
    minorSecondIsEnabled.onClick = [this]
    {
        audioProcessor.setMinorSecondIsEnabled();
    };
    //---------------------- MAJOR SECOND IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(majorSecondIsEnabled);
    majorSecondIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MAJOR_SECOND_IS_ENABLED", majorSecondIsEnabled);
    majorSecondIsEnabled.onClick = [this]
    {
        audioProcessor.setMajorSecondIsEnabled();
    };
    //---------------------- MINOR THIRD IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(minorThirdIsEnabled);
    minorThirdIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MINOR_THIRD_IS_ENABLED", minorThirdIsEnabled);
    minorThirdIsEnabled.onClick = [this]
    {
        audioProcessor.setMinorThirdIsEnabled();
    };
    //---------------------- MAJOR THIRD IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(majorThirdIsEnabled);
    majorThirdIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MAJOR_THIRD_IS_ENABLED", majorThirdIsEnabled);
    majorThirdIsEnabled.onClick = [this]
    {
        audioProcessor.setMajorThirdIsEnabled();
    };
    //---------------------- PERFECT FOURTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(perfectFourthIsEnabled);
    perfectFourthIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PERFECT_FOURTH_IS_ENABLED", perfectFourthIsEnabled);
    perfectFourthIsEnabled.onClick = [this]
    {
        audioProcessor.setPerfectFourthIsEnabled();
    };
    //---------------------- DIMINISHED FIFTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(diminishedFifthIsEnabled);
    diminishedFifthIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "DIMINISHED_FIFTH_IS_ENABLED", diminishedFifthIsEnabled);
    diminishedFifthIsEnabled.onClick = [this]
    {
        audioProcessor.setDiminishedFifthIsEnabled();
    };
    //---------------------- PERFECT FIFTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(perfectFifthIsEnabled);
    perfectFifthIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PERFECT_FIFTH_IS_ENABLED", perfectFifthIsEnabled);
    perfectFifthIsEnabled.onClick = [this]
    {
        audioProcessor.setPerfectFifthIsEnabled();
    };
    //---------------------- MINOR SIXTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(minorSixthIsEnabled);
    minorSixthIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MINOR_SIXTH_IS_ENABLED", minorSixthIsEnabled);
    minorSixthIsEnabled.onClick = [this]
    {
        audioProcessor.setMinorSixthIsEnabled();
    };
    //---------------------- MAJOR SIXTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(majorSixthIsEnabled);
    majorSixthIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MAJOR_SIXTH_IS_ENABLED", majorSixthIsEnabled);
    majorSixthIsEnabled.onClick = [this]
    {
        audioProcessor.setMajorSixthIsEnabled();
    };
    //---------------------- MINOR SEVENTH IS ENABLED BUTTON -------------------------------------------------------------------------------------------
    addAndMakeVisible(minorSeventhIsEnabled);
    minorSeventhIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MINOR_SEVENTH_IS_ENABLED", minorSeventhIsEnabled);
    minorSeventhIsEnabled.onClick = [this]
    {
        audioProcessor.setMinorSeventhIsEnabled();
    };
    //---------------------- MAJOR SEVENTH IS ENABLED BUTTON -----------------------------------------------------------------------------------------------
    addAndMakeVisible(majorSeventhIsEnabled);
    majorSeventhIsEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MAJOR_SEVENTH_IS_ENABLED", majorSeventhIsEnabled);
    majorSeventhIsEnabled.onClick = [this]
    {
        audioProcessor.setMajorSeventhIsEnabled();
    };

    //---------------------- TUNING CHOICE COMBO BOX -------------------------------------------------------------------------------------------------------
    tuningChoice.addItemList(audioProcessor.apvts.getParameter("TUNING_CHOICE")->getAllValueStrings(), 1);
    tuningChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TUNING_CHOICE", tuningChoice);
    tuningChoiceLabel.setText("Tuning", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(tuningChoiceLabel);
    addAndMakeVisible(tuningChoice);
    tuningChoice.onChange = [this]
    {
        setTuning(tuningChoice.getSelectedId() - 1);
    };
    

    //--------------------------------------------------------------------------------------------------------------------------------------------------------

    addAndMakeVisible(numOfActiveVoices);

    startTimer(50);

    setEffectSliderVisibility(effectChoice.getSelectedId() - 1);

    
    
}

MPEVstProjectAudioProcessorEditor::~MPEVstProjectAudioProcessorEditor()
{
}

//==============================================================================
void MPEVstProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)


    juce::Image logo = juce::ImageCache::getFromMemory(BinaryData::VST_Logo_2_jpg, BinaryData::VST_Logo_2_jpgSize);
    juce::Colour moduleRectangleOutlineColour(0, 0, 0);
    juce::Colour moduleRectangleFillColour(136, 0, 21);
    juce::Colour labelBackgroundColour(70, 0, 0);
    juce::Colour popupBackgroundColour(33, 33, 33);
    juce::Colour popupOutlineColour(255, 255, 255);
    juce::Colour sliderFillColour(33, 33, 33);
    juce::Colour sliderOutlineColour(70, 70, 70);
    juce::Colour sliderThumbColour(200, 200, 200);
    juce::Colour buttonAndComboboxColour(33, 33, 33);
    

    g.fillAll(juce::Colour(70, 0, 0));  // Background Colour

    g.drawImageAt(logo, 410, 590);  //  Drawing the Logo

    g.setColour(moduleRectangleFillColour);          // OSC Rectangle
    g.fillRoundedRectangle(10, 10, 215, 370, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(10, 10, 215, 370, 5, 2);



    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(20, 110, 195, 20, 5);    // OSC 1 Parameters Label Box
    g.fillRoundedRectangle(20, 230, 195, 20, 5);    // OSC 2 Parameters Label Box
    g.fillRoundedRectangle(20, 350, 195, 20, 5);    // OSC 3 Parameters Label Box
    g.fillRoundedRectangle(20, 20, 45, 20, 5);      // OSC 1 Label Box
    g.fillRoundedRectangle(20, 140, 45, 20, 5);      // OSC 1 Label Box
    g.fillRoundedRectangle(20, 260, 45, 20, 5);      // OSC 1 Label Box


    g.setColour(moduleRectangleFillColour);          // Noise OSC Rectangle
    g.fillRoundedRectangle(235, 10, 130, 100, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(235, 10, 130, 100, 5, 2);


    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(245, 80, 110, 20, 5);    // Noise OSC Label Box

    g.setColour(moduleRectangleFillColour);          // ADSR Rectangle
    g.fillRoundedRectangle(235, 120, 130, 260, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(235, 120, 130, 260, 5, 2);

    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(245, 130, 110, 20, 5);    // ADSR Label Box

    g.setColour(moduleRectangleFillColour);          // Filter Rectangle
    g.fillRoundedRectangle(375, 10, 400, 280, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(375, 10, 400, 280, 5, 2);

    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(385, 20, 110, 20, 5);    // Filter Type Label Box
    g.fillRoundedRectangle(385, 50, 110, 20, 5);    // Filter Mode Label Box
    g.fillRoundedRectangle(385, 80, 110, 20, 5);    // Filter LFO Waveform Label Box
    g.fillRoundedRectangle(385, 170, 110, 20, 5);    // Filter Cutoff Label Box
    g.fillRoundedRectangle(505, 170, 110, 20, 5);    // Filter Resonance Label Box
    g.fillRoundedRectangle(385, 260, 110, 20, 5);    // Filter LFO Rate Label Box
    g.fillRoundedRectangle(505, 260, 110, 20, 5);    // Filter LFO Depth Label Box
    g.fillRoundedRectangle(625, 20, 140, 20, 5);    // Filter ADSR Label Box

    g.setColour(moduleRectangleFillColour);          // Effects Rectangle
    g.fillRoundedRectangle(10, 390, 355, 190, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(10, 390, 355, 190, 5, 2);

    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(20, 400, 110, 20, 5);    // Effects Label Box
    g.fillRoundedRectangle(20, 430, 110, 20, 5);    // 1st Effect parameter Label Box
    g.fillRoundedRectangle(20, 460, 110, 20, 5);    // 2st Effect parameter Label Box
    g.fillRoundedRectangle(20, 490, 110, 20, 5);    // 3st Effect parameter Label Box
    g.fillRoundedRectangle(20, 520, 110, 20, 5);    // 4st Effect parameter Label Box
    g.fillRoundedRectangle(20, 550, 110, 20, 5);    // 5st Effect parameter Label Box

    g.setColour(moduleRectangleFillColour);          // Tuning Rectangle
    g.fillRoundedRectangle(375, 300, 400, 280, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(375, 300, 400, 280, 5, 2);

    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(385, 310, 40, 20, 5);    // Root Label Box
    g.fillRoundedRectangle(505, 310, 70, 20, 5);    // Tuning Label Box

    g.fillRoundedRectangle(385, 340, 90, 20, 5);    // Root Note Label Box
    g.fillRoundedRectangle(482, 340, 90, 20, 5);    // Minor 2nd Label Box
    g.fillRoundedRectangle(579, 340, 90, 20, 5);    // Major 2nd Label Box
    g.fillRoundedRectangle(676, 340, 90, 20, 5);    // Minor 3rd Label Box
    g.fillRoundedRectangle(385, 420, 90, 20, 5);    // Major 3rd Label Box
    g.fillRoundedRectangle(482, 420, 90, 20, 5);    // Perfect 4th Label Box
    g.fillRoundedRectangle(579, 420, 90, 20, 5);    // Dimimnished 5th Label Box
    g.fillRoundedRectangle(676, 420, 90, 20, 5);    // Perfect 5th Label Box
    g.fillRoundedRectangle(385, 500, 90, 20, 5);    // Minor 6th Label Box
    g.fillRoundedRectangle(482, 500, 90, 20, 5);    // Major 6th Label Box
    g.fillRoundedRectangle(579, 500, 90, 20, 5);    // Minor 7th Label Box
    g.fillRoundedRectangle(676, 500, 90, 20, 5);    // Major 7th Label Box

    g.setColour(moduleRectangleFillColour);          // Master Volume/Voice Count Rectangle
    g.fillRoundedRectangle(10, 590, 390, 70, 5);
    g.setColour(moduleRectangleOutlineColour);
    g.drawRoundedRectangle(10, 590, 390, 70, 5, 2);

    g.setColour(labelBackgroundColour);
    g.fillRoundedRectangle(20, 600, 110, 20, 5);    // Master Volume Label Box
    g.fillRoundedRectangle(20, 630, 110, 20, 5);    // Max Voices Label Box
    g.fillRoundedRectangle(248, 630, 142, 20, 5);    // Active Voices Label Box




    lookAndFeel.setColour(juce::ComboBox::ColourIds::backgroundColourId, buttonAndComboboxColour);
    lookAndFeel.setColour(juce::ComboBox::ColourIds::arrowColourId, popupOutlineColour);

    lookAndFeel.setColour(juce::PopupMenu::ColourIds::backgroundColourId, popupBackgroundColour);
    lookAndFeel.setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, moduleRectangleFillColour);

    lookAndFeel.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, sliderFillColour);
    lookAndFeel.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, sliderOutlineColour);
    lookAndFeel.setColour(juce::Slider::ColourIds::thumbColourId, sliderThumbColour);

    lookAndFeel.setColour(juce::Slider::ColourIds::trackColourId, sliderFillColour);

    lookAndFeel.setColour(juce::BubbleComponent::ColourIds::backgroundColourId, popupBackgroundColour);   

    lookAndFeel.setColour(juce::TextButton::ColourIds::buttonColourId, buttonAndComboboxColour);

    lookAndFeel.setColour(juce::TooltipWindow::ColourIds::backgroundColourId, popupBackgroundColour);
    lookAndFeel.setColour(juce::TooltipWindow::ColourIds::outlineColourId, popupOutlineColour);




    lookAndFeel.setDefaultSansSerifTypefaceName("OCR A Extended");
    setLookAndFeel(&lookAndFeel);

    //getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);

}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


    //===================== OSC 1 UI PLACEMENT =============================================================
    osc1Choice.setBounds(75, 20, 140, 20);
    osc1ChoiceLabel.setBounds(20, 19, 60, 20);
    osc1LevelSlider.setBounds(20, 40, 75, 75);
    osc1LevelLabel.setBounds(28, 109, 75, 20);
    osc1DetuneSlider.setBounds(95, 40, 75, 75);
    osc1DetuneLabel.setBounds(105, 109, 75, 20);
    osc1SemitonesSlider.setBounds(170, 50, 45, 50);
    osc1SemitonesLabel.setBounds(172, 109, 75, 20);
    //===================== OSC 2 UI PLACEMENT =============================================================
    osc2Choice.setBounds(75, 140, 140, 20);
    osc2ChoiceLabel.setBounds(20, 139, 60, 20);
    osc2LevelSlider.setBounds(20, 160, 75, 75);
    osc2LevelLabel.setBounds(28, 229, 75, 20);
    osc2DetuneSlider.setBounds(95, 160, 75, 75);
    osc2DetuneLabel.setBounds(105, 229, 75, 20);
    osc2SemitonesSlider.setBounds(170, 170, 45, 50);
    osc2SemitonesLabel.setBounds(172, 229, 75, 20);
    //===================== OSC 3 UI PLACEMENT =============================================================
    osc3Choice.setBounds(75, 260, 140, 20);
    osc3ChoiceLabel.setBounds(20, 259, 60, 20);
    osc3LevelSlider.setBounds(20, 280, 75, 75);
    osc3LevelLabel.setBounds(28, 349, 75, 20);
    osc3DetuneSlider.setBounds(95, 280, 75, 75);
    osc3DetuneLabel.setBounds(105, 349, 75, 20);
    osc3SemitonesSlider.setBounds(170, 290, 45, 50);
    osc3SemitonesLabel.setBounds(172, 349, 75, 20);
    //===================== NOISE OSC UI PLACEMENT =============================================================
    noiseLevelSlider.setBounds(263, 10, 75, 75);
    noiseLevelLabel.setBounds(278, 79, 130, 20);
    //===================== VOICE ADSR SLIDER PLACEMENT =============================================================
    voiceAttackSlider.setBounds(245, 160, 20, 210);
    voiceAttackLabel.setBounds(245, 129, 30, 20);
    voiceDecaySlider.setBounds(275, 160, 20, 210);
    voiceDecayLabel.setBounds(275, 129, 30, 20);
    voiceSustainSlider.setBounds(305, 160, 20, 210);
    voiceSustainLabel.setBounds(305, 129, 30, 20);
    voiceReleaseSlider.setBounds(335, 160, 20, 210);
    voiceReleaseLabel.setBounds(335, 129, 30, 20);
    //===================== FILTER UI PLACEMENT =============================================================
    filterCutoffSlider.setBounds(403, 100, 75, 75);
    filterCutoffLabel.setBounds(415, 169, 110, 20);
    filterResonanceSlider.setBounds(523, 100, 75, 75);
    filterResonanceLabel.setBounds(522, 169, 110, 20);
    filterTypeChoice.setBounds(505, 20, 110, 20);
    filterTypeChoiceLabel.setBounds(400, 19, 130, 20);
    filterModeChoice.setBounds(505, 50, 110, 20);
    filterModeChoiceLabel.setBounds(400, 49, 130, 20);
    //===================== FILTER LFO UI PLACEMENT =============================================================
    filterLFOOscChoice.setBounds(505, 80, 110, 20);
    filterLFOOscChoiceLabel.setBounds(390, 79, 130, 20);
    filterLFORateSlider.setBounds(403, 190, 75, 75);
    filterLFORateLabel.setBounds(408, 259, 130, 20);
    filterLFODepthSlider.setBounds(523, 190, 75, 75);
    filterLFODepthLabel.setBounds(522, 259, 90, 20);
    //===================== FILTER ADSR UI PLACEMENT =============================================================
    filterAttackSlider.setBounds(625, 50, 20, 230);
    filterAttackLabel.setBounds(625, 19, 30, 20);
    filterDecaySlider.setBounds(655, 50, 20, 230);
    filterDecayLabel.setBounds(655, 19, 30, 20);
    filterSustainSlider.setBounds(685, 50, 20, 230);
    filterSustainLabel.setBounds(685, 19, 30, 20);
    filterReleaseSlider.setBounds(715, 50, 20, 230);
    filterReleaseLabel.setBounds(715, 19, 30, 20);
    filterADSRAmplitudeSlider.setBounds(745, 50, 20, 230);
    filterADSRAmplitudeLabel.setBounds(745, 19, 30, 20);
    //===================== EFFECT CHOICE UI PLACEMENT =============================================================
    effectChoice.setBounds(140, 400, 215, 20);
    effectChoiceLabel.setBounds(48, 399, 150, 20);
    //===================== CHORUS UI PLACEMENT =============================================================
    chorusCentreDelaySlider.setBounds(140, 430, 215, 20);
    chorusCentreDelayLabel.setBounds(29, 429, 110, 20);
    chorusDepthSlider.setBounds(140, 460, 215, 20);
    chorusDepthLabel.setBounds(52, 459, 110, 20);
    chorusRateSlider.setBounds(140, 490, 215, 20);
    chorusRateLabel.setBounds(55, 489, 110, 20);
    chorusFeedbackSlider.setBounds(140, 520, 215, 20);
    chorusFeedbackLabel.setBounds(40, 519, 110, 20);
    chorusMixSlider.setBounds(140, 550, 215, 20);
    chorusMixLabel.setBounds(59, 549, 110, 20);
    //===================== PHASER UI PLACEMENT =============================================================
    phaserCentreFrequencySlider.setBounds(140, 430, 215, 20);
    phaserCentreFrequencyLabel.setBounds(32, 429, 110, 20);
    phaserDepthSlider.setBounds(140, 460, 215, 20);
    phaserDepthLabel.setBounds(52, 459, 110, 20);
    phaserRateSlider.setBounds(140, 490, 215, 20);
    phaserRateLabel.setBounds(55, 489, 110, 20);
    phaserFeedbackSlider.setBounds(140, 520, 215, 20);
    phaserFeedbackLabel.setBounds(40, 519, 110, 20);
    phaserMixSlider.setBounds(140, 550, 215, 20);
    phaserMixLabel.setBounds(59, 549, 110, 20);
    //===================== REVERB UI PLACEMENT =============================================================
    reverbRoomSizeSlider.setBounds(140, 430, 215, 20);
    reverbRoomSizeLabel.setBounds(37, 429, 110, 20);
    reverbWidthSlider.setBounds(140, 460, 215, 20);
    reverbWidthLabel.setBounds(52, 459, 110, 20);
    reverbDampingSlider.setBounds(140, 490, 215, 20);
    reverbDampingLabel.setBounds(42, 489, 110, 20);
    reverbDrySlider.setBounds(140, 520, 215, 20);
    reverbDryLabel.setBounds(59, 519, 110, 20);
    reverbWetSlider.setBounds(140, 550, 215, 20);
    reverbWetLabel.setBounds(57, 549, 110, 20);
    //===================== TUNING UI PLACEMENT =============================================================
    rootNoteChoice.setBounds(435, 310, 60, 20);
    rootNoteChoiceLabel.setBounds(385, 309, 70, 20);

    tuningChoice.setBounds(585, 310, 180, 20);
    tuningChoiceLabel.setBounds(514, 309, 110, 20);
   
    rootSlider.setBounds(385, 363, 70, 47);
    minorSecondSlider.setBounds(482, 363, 70, 47);
    majorSecondSlider.setBounds(579, 363, 70, 47);
    minorThirdSlider.setBounds(676, 363, 70, 47);
    majorThirdSlider.setBounds(385, 443, 70, 47);
    perfectFourthSlider.setBounds(482, 443, 70, 47);
    diminishedFifthSlider.setBounds(579, 443, 70, 47);
    perfectFifthSlider.setBounds(676, 443, 70, 47);
    minorSixthSlider.setBounds(385, 523, 70, 47);
    majorSixthSlider.setBounds(482, 523, 70, 47);
    minorSeventhSlider.setBounds(579, 523, 70, 47);
    majorSeventhSlider.setBounds(676, 523, 70, 47);

    rootLabel.setBounds(410, 339, 70, 20);
    minorSecondLabel.setBounds(492, 339, 70, 20);
    majorSecondLabel.setBounds(589, 339, 70, 20);
    minorThirdLabel.setBounds(686, 339, 70, 20);
    majorThirdLabel.setBounds(395, 419, 70, 20);
    perfectFourthLabel.setBounds(492, 419, 70, 20);
    diminishedFifthLabel.setBounds(593, 419, 70, 20);
    perfectFifthLabel.setBounds(686, 419, 70, 20);
    minorSixthLabel.setBounds(395, 499, 70, 20);
    majorSixthLabel.setBounds(492, 499, 70, 20);
    minorSeventhLabel.setBounds(589, 499, 70, 20);
    majorSeventhLabel.setBounds(686, 499, 70, 20);
    
    rootIsEnabled.setBounds(454, 368, 22, 20);
    minorSecondIsEnabled.setBounds(551, 368, 22, 20);
    majorSecondIsEnabled.setBounds(648, 368, 22, 20);
    minorThirdIsEnabled.setBounds(745, 368, 22, 20);
    majorThirdIsEnabled.setBounds(454, 448, 22, 20);
    perfectFourthIsEnabled.setBounds(551, 448, 22, 20);
    diminishedFifthIsEnabled.setBounds(648, 448, 22, 20);
    perfectFifthIsEnabled.setBounds(745, 448, 22, 20);
    minorSixthIsEnabled.setBounds(454, 528, 22, 20);
    majorSixthIsEnabled.setBounds(551, 528, 22, 20);
    minorSeventhIsEnabled.setBounds(648, 528, 22, 20);
    majorSeventhIsEnabled.setBounds(745, 528, 22, 20);

    //===================== MASTER VOLUME SLIDER PLACEMENT =============================================================   
    masterSlider.setBounds(140, 600, 250, 20);
    masterLabel.setBounds(25, 599, 100, 20);
    //===================== NUMBER OF VOICES UI PLACEMENT =============================================================
    monophonic.setBounds(138, 630, 100, 20);
    monophonicLabel.setBounds(35, 629, 100, 20);

    numOfActiveVoices.setBounds(262, 629, 120, 20);


}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::setEffectSliderVisibility(int mode) {

    chorusCentreDelaySlider.setVisible(false);
    chorusDepthSlider.setVisible(false);
    chorusRateSlider.setVisible(false);
    chorusFeedbackSlider.setVisible(false);
    chorusMixSlider.setVisible(false);

    chorusCentreDelayLabel.setVisible(false);
    chorusDepthLabel.setVisible(false);
    chorusRateLabel.setVisible(false);
    chorusFeedbackLabel.setVisible(false);
    chorusMixLabel.setVisible(false);

    phaserCentreFrequencySlider.setVisible(false);
    phaserDepthSlider.setVisible(false);
    phaserRateSlider.setVisible(false);
    phaserFeedbackSlider.setVisible(false);
    phaserMixSlider.setVisible(false);

    phaserCentreFrequencyLabel.setVisible(false);
    phaserDepthLabel.setVisible(false);
    phaserRateLabel.setVisible(false);
    phaserFeedbackLabel.setVisible(false);
    phaserMixLabel.setVisible(false);

    reverbDampingSlider.setVisible(false);
    reverbRoomSizeSlider.setVisible(false);
    reverbWidthSlider.setVisible(false);
    reverbWetSlider.setVisible(false);
    reverbDrySlider.setVisible(false);

    reverbDampingLabel.setVisible(false);
    reverbRoomSizeLabel.setVisible(false);
    reverbWidthLabel.setVisible(false);
    reverbWetLabel.setVisible(false);
    reverbDryLabel.setVisible(false);

    switch (mode)
    {
    case 0:
        chorusCentreDelaySlider.setVisible(true);
        chorusDepthSlider.setVisible(true);
        chorusRateSlider.setVisible(true);
        chorusFeedbackSlider.setVisible(true);
        chorusMixSlider.setVisible(true);
        chorusCentreDelayLabel.setVisible(true);
        chorusDepthLabel.setVisible(true);
        chorusRateLabel.setVisible(true);
        chorusFeedbackLabel.setVisible(true);
        chorusMixLabel.setVisible(true);
        break;
    case 1:
        phaserCentreFrequencySlider.setVisible(true);
        phaserDepthSlider.setVisible(true);
        phaserRateSlider.setVisible(true);
        phaserFeedbackSlider.setVisible(true);
        phaserMixSlider.setVisible(true);
        phaserCentreFrequencyLabel.setVisible(true);
        phaserDepthLabel.setVisible(true);
        phaserRateLabel.setVisible(true);
        phaserFeedbackLabel.setVisible(true);
        phaserMixLabel.setVisible(true);
        break;
    case 2:
        reverbDampingSlider.setVisible(true);
        reverbRoomSizeSlider.setVisible(true);
        reverbWidthSlider.setVisible(true);
        reverbWetSlider.setVisible(true);
        reverbDrySlider.setVisible(true);
        reverbDampingLabel.setVisible(true);
        reverbRoomSizeLabel.setVisible(true);
        reverbWidthLabel.setVisible(true);
        reverbWetLabel.setVisible(true);
        reverbDryLabel.setVisible(true);
        break;

    default:
        break;
    }

}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::setTuning(int tuning) {

    switch (tuning)
    {
    case 0:     // 12 TET
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 1:     // 11 TET
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, false, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 109, 218, 327, 436, 545, 0, 654, 764, 873, 982, 1091 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 2:     // 10 TET
    {
        bool enableNotesArray[] = { true, true, false, true, true, true, true, true, true, false, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 120, 0, 240, 360, 480, 600, 720, 840, 0, 960, 1080 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 3:     // 9 TET
    {
        bool enableNotesArray[] = { true, false, true, true, true, true, false, true, false, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 0, 133, 267, 400, 533, 0, 667, 0, 800, 933, 1067 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 4:     // 8 TET
    {
        bool enableNotesArray[] = { true, false, true, true, true, true, false, true, false, true, false, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 0, 150, 300, 450, 600, 0, 750, 0, 900, 0, 1050 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 5:     // 7 TET
    {
        bool enableNotesArray[] = { true, false, true, true, false, true, false, true, false, true, false, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 0, 171, 343, 0, 514, 0, 686, 0, 857, 0, 1029 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 6:     // Pythagore
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 90, 204, 294, 408, 498, 612, 702, 792, 906, 996, 1110 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 7:     // Zarlino
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 71, 204, 294, 386, 498, 590, 702, 773, 884, 996, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 8:     // Well temperament
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 90, 195, 294, 389, 498, 588, 697, 792, 892, 996, 1091 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 9:     // Werckmeister III
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 90, 192, 294, 390, 498, 588, 696, 792, 888, 996, 1092 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 10:    // Meantone (1/4 comma)
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 76, 193, 310, 386, 503, 579, 697, 773, 890, 1007, 1083 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 11:    // Meantone (1/3-comma)
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 63, 190, 316, 379, 505, 569, 695, 758, 884, 1010, 1074 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 12:    // Meantone (1/5-comma)
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 84, 195, 307, 391, 502, 586, 698, 781, 893, 1005, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 13:    // Meantone (1/6-comma)
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 89, 197, 305, 393, 502, 590, 698, 787, 895, 1003, 1092 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 14:    // Meantone (2/7-comma)
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 71, 192, 313, 383, 504, 575, 696, 766, 887, 1008, 1079 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 15:    // Kirnberger III
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 90, 193, 294, 386, 498, 590, 697, 792, 890, 996, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 16:    // Vallotti & Young
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 94, 196, 298, 392, 502, 592, 698, 796, 894, 1000, 1090 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 17:    // Neidhardt III
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 96, 196, 298, 394, 498, 596, 698, 796, 894, 998, 1096 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 18:    // Just intonation
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 112, 204, 316, 386, 498, 582, 702, 814, 884, 1018, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 19:    // Fokker
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 119, 204, 267, 386, 498, 590, 702, 821, 884, 969, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    case 20:    // Super Just
    {
        bool enableNotesArray[] = { true, true, true, true, true, true, true, true, true, true, true, true };
        enableNotes(enableNotesArray);

        int noteCentsArray[] = { 0, 105, 204, 316, 386, 498, 551, 702, 840, 884, 969, 1088 };
        setNoteCents(noteCentsArray);

        break;
    }
    default:
        break;
    }

}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::enableNotes(bool notesEnabled[]) {

    if (rootIsEnabled.getToggleState() != notesEnabled[0]) rootIsEnabled.triggerClick();
    if (minorSecondIsEnabled.getToggleState() != notesEnabled[1]) minorSecondIsEnabled.triggerClick();
    if (majorSecondIsEnabled.getToggleState() != notesEnabled[2]) majorSecondIsEnabled.triggerClick();
    if (minorThirdIsEnabled.getToggleState() != notesEnabled[3]) minorThirdIsEnabled.triggerClick();
    if (majorThirdIsEnabled.getToggleState() != notesEnabled[4]) majorThirdIsEnabled.triggerClick();
    if (perfectFourthIsEnabled.getToggleState() != notesEnabled[5]) perfectFourthIsEnabled.triggerClick();
    if (diminishedFifthIsEnabled.getToggleState() != notesEnabled[6]) diminishedFifthIsEnabled.triggerClick();
    if (perfectFifthIsEnabled.getToggleState() != notesEnabled[7]) perfectFifthIsEnabled.triggerClick();
    if (minorSixthIsEnabled.getToggleState() != notesEnabled[8]) minorSixthIsEnabled.triggerClick();
    if (majorSixthIsEnabled.getToggleState() != notesEnabled[9]) majorSixthIsEnabled.triggerClick();
    if (minorSeventhIsEnabled.getToggleState() != notesEnabled[10]) minorSeventhIsEnabled.triggerClick();
    if (majorSeventhIsEnabled.getToggleState() != notesEnabled[11]) majorSeventhIsEnabled.triggerClick();

}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::setNoteCents(int noteCents[]) {

    rootSlider.setValue(noteCents[0]);
    minorSecondSlider.setValue(noteCents[1]);
    majorSecondSlider.setValue(noteCents[2]);
    minorThirdSlider.setValue(noteCents[3]);
    majorThirdSlider.setValue(noteCents[4]);
    perfectFourthSlider.setValue(noteCents[5]);
    diminishedFifthSlider.setValue(noteCents[6]);
    perfectFifthSlider.setValue(noteCents[7]);
    minorSixthSlider.setValue(noteCents[8]);
    majorSixthSlider.setValue(noteCents[9]);
    minorSeventhSlider.setValue(noteCents[10]);
    majorSeventhSlider.setValue(noteCents[11]);

}
//======================================================================================================================
void MPEVstProjectAudioProcessorEditor::timerCallback() {
    numOfActiveVoices.setText(juce::String(audioProcessor.getNumActiveVoices()) + " Voices Playing", juce::NotificationType::dontSendNotification);
}
