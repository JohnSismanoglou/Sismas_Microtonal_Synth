/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MPEAudioEngine.h"

//==============================================================================
MPEVstProjectAudioProcessor::MPEVstProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this,nullptr,"ValueTreeState",createParameters())
#endif
{
}

MPEVstProjectAudioProcessor::~MPEVstProjectAudioProcessor()
{
}

//==============================================================================
const juce::String MPEVstProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MPEVstProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MPEVstProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MPEVstProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MPEVstProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MPEVstProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MPEVstProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MPEVstProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MPEVstProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void MPEVstProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MPEVstProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    MPEVstProjectAudioProcessor::sampleRate = sampleRate;
    MPEVstProjectAudioProcessor::samplesPerBlock = samplesPerBlock;

    //================================== PREPARING MASTER VOLUME ===================================================
    masterVolume.setRampDurationSeconds(0.05);
    masterVolume.setGainLinear(apvts.getRawParameterValue("MASTER")->load());
    masterVolume.prepare({ sampleRate,(juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() });

    //================================== PREPARING AUDIO ENGINE ===================================================
    mpeAudioEngine.prepare({ sampleRate,(juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() }, &apvts);

    
}

void MPEVstProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MPEVstProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MPEVstProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels(); 
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    // set parameters in process block to make automation work as intended (if i dont do that automation works only if the GUI of the plugin is open)
    setMasterVolume();
    setVoiceAttack();
    setVoiceDecay();
    setVoiceSustain();
    setVoiceRelease();
    setOsc1Level();
    setOsc1Semitones();
    setOsc1Detune();
    setOsc2Level();
    setOsc2Semitones();
    setOsc2Detune();
    setOsc3Level();
    setOsc3Semitones();
    setOsc3Detune();
    setNoiseLevel();
    setFilterCutoff();
    setFilterResonance();
    setFilterType();
    setFilterMode();
    setFilterAttack();
    setFilterDecay();
    setFilterSustain();
    setFilterRelease();
    setFilterADSRAmplitude();
    setFilterLFOOsc();
    setFilterLFORate();
    setFilterLFODepth();
    setChorusRate();
    setChorusDepth();
    setChorusFeedback();
    setChorusCentreDelay();
    setChorusMix();
    setPhaserRate();
    setPhaserDepth();
    setPhaserFeedback();
    setPhaserCentreFrequency();
    setPhaserMix();
    setReverbRoomSize();
    setReverbWidth();
    setReverbDamping();
    setReverbWet();
    setReverbDry();
    setRootNoteChoice();
    setRootCents();
    setMinorSecondCents();
    setMajorSecondCents();
    setMinorThirdCents();
    setMajorThirdCents();
    setPerfectFourthCents();
    setDiminishedFifthCents();
    setPerfectFifthCents();
    setMinorSixthCents();
    setMajorSixthCents();
    setMinorSeventhCents();
    setMajorSeventhCents();
    setRootIsEnabled();
    setMinorSecondIsEnabled();
    setMajorSecondIsEnabled();
    setMinorThirdIsEnabled();
    setMajorThirdIsEnabled();
    setPerfectFourthIsEnabled();
    setDiminishedFifthIsEnabled();
    setPerfectFifthIsEnabled();
    setMinorSixthIsEnabled();
    setMajorSixthIsEnabled();
    setMinorSeventhIsEnabled();
    setMajorSeventhIsEnabled();
    //==========================================================================================
    


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
    }
    
    mpeAudioEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto contextToUse = juce::dsp::ProcessContextReplacing<float>(block);

    masterVolume.process(contextToUse);
    
}

//==============================================================================
bool MPEVstProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MPEVstProjectAudioProcessor::createEditor()
{
    return new MPEVstProjectAudioProcessorEditor (*this);
}

//==============================================================================
void MPEVstProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MPEVstProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MPEVstProjectAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MPEVstProjectAudioProcessor::createParameters(){

    //std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    //==================================== OSC CHOICES ===================================================
    juce::StringArray oscChoices;
    oscChoices.add("Disabled");
    oscChoices.add("Sine");
    oscChoices.add("Saw");
    oscChoices.add("Square");
    oscChoices.add("Triangle");
    oscChoices.add("Triangle From Sine Harmonics");
    oscChoices.add("Square From Sine Harmonics");
    oscChoices.add("Saw From Sine Harmonics");
    oscChoices.add("Square Triangle");
    oscChoices.add("Saw 2");
    oscChoices.add("Saw In Phase");
    oscChoices.add("Triangle Real");
    oscChoices.add("Smooth Square");
    oscChoices.add("Filtered Square");
    oscChoices.add("Organ");
    oscChoices.add("Lofi Triangle");
    oscChoices.add("Really Lofi Triangle");

    //==================================== FILTER TYPE CHOICES ===================================================
    juce::StringArray filterChoices;
    filterChoices.add("Lowpass");
    filterChoices.add("Bandpass");
    filterChoices.add("Highpass");

    //==================================== FILTER MODE CHOICES ===================================================
    juce::StringArray filterModeChoices;
    filterModeChoices.add("Static Filter");
    filterModeChoices.add("ADSR Filter");
    filterModeChoices.add("Keyboard Tracked Filter");
    filterModeChoices.add("LFO Filter");

    //==================================== FILTER LFO WAVEFORM CHOICES ===================================================
    juce::StringArray filterLFOWaveformChoices;
    filterLFOWaveformChoices.add("Disabled");
    filterLFOWaveformChoices.add("Sine");
    filterLFOWaveformChoices.add("Triangle");
    filterLFOWaveformChoices.add("Saw");
    filterLFOWaveformChoices.add("Inverted Saw");
    filterLFOWaveformChoices.add("Square");
    filterLFOWaveformChoices.add("Inverted Square");

    //==================================== EFFECT CHOICES ===================================================
    juce::StringArray effectChoices;
    effectChoices.add("Chorus");
    effectChoices.add("Phaser");
    effectChoices.add("Reverb");

    //==================================== ROOT NOTE CHOICES ===================================================
    juce::StringArray rootNoteChoices;
    rootNoteChoices.add("C");
    rootNoteChoices.add("C#");
    rootNoteChoices.add("D");
    rootNoteChoices.add("D#");
    rootNoteChoices.add("E");
    rootNoteChoices.add("F");
    rootNoteChoices.add("F#");
    rootNoteChoices.add("G");
    rootNoteChoices.add("G#");
    rootNoteChoices.add("A");
    rootNoteChoices.add("A#");
    rootNoteChoices.add("B");

    //==================================== TUNING CHOICES ===================================================
    juce::StringArray tuningChoices;
    tuningChoices.add("12 TET");
    tuningChoices.add("11 TET");
    tuningChoices.add("10 TET");
    tuningChoices.add("9 TET");
    tuningChoices.add("8 TET");
    tuningChoices.add("7 TET");
    tuningChoices.add("Pythagore");
    tuningChoices.add("Zarlino");
    tuningChoices.add("Well temperament");
    tuningChoices.add("Werckmeister III");
    tuningChoices.add("Meantone (1/4 comma)");
    tuningChoices.add("Meantone (1/3-comma)");
    tuningChoices.add("Meantone (1/5-comma)");
    tuningChoices.add("Meantone (1/6-comma)");
    tuningChoices.add("Meantone (2/7-comma)");
    tuningChoices.add("Kirnberger III");
    tuningChoices.add("Vallotti & Young");
    tuningChoices.add("Neidhardt III");
    tuningChoices.add("Just intonation");
    tuningChoices.add("Fokker");
    tuningChoices.add("Super Just");

    //==================================================== MASTER VOLUME PARAMETER ===================================================
    params.add(std::make_unique<juce::AudioParameterFloat>("MASTER", "Master", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.75f));
    //==================================================== VOICE ADSR PARAMETERS ===================================================
    params.add(std::make_unique<juce::AudioParameterFloat>("VOICE_ATTACK", "Attack", 0.01f, 2.0f, 0.01f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VOICE_DECAY", "Decay", 0.0005f, 2.0f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VOICE_SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VOICE_RELEASE", "Release", 0.01f, 2.0f, 0.01f));
    //==================================================== OSC 1 PARAMETERS ===================================================
    params.add(std::make_unique<juce::AudioParameterChoice>("OSC_1", "Oscilator 1", oscChoices, 1));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_1_LEVEL", "Oscilator 1 level", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.75f));
    params.add(std::make_unique<juce::AudioParameterInt>("OSC_1_SEMITONES", "Oscilator 1 Semitones", -24, 24, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_1_DETUNE", "Oscilator 1 Detune", -50.0f, 50.0f, 0));
    //==================================================== OSC 2 PARAMETERS ===================================================
    params.add(std::make_unique<juce::AudioParameterChoice>("OSC_2", "Oscilator 2", oscChoices, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_2_LEVEL", "Oscilator 2 level", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.75f));
    params.add(std::make_unique<juce::AudioParameterInt>("OSC_2_SEMITONES", "Oscilator 2 Semitones", -24, 24, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_2_DETUNE", "Oscilator 2 Detune", -50.0f, 50.0f, 0));
    //==================================================== OSC 3 PARAMETERS ===================================================
    params.add(std::make_unique<juce::AudioParameterChoice>("OSC_3", "Oscilator 3", oscChoices, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_3_LEVEL", "Oscilator 3 level", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.75f));
    params.add(std::make_unique<juce::AudioParameterInt>("OSC_3_SEMITONES", "Oscilator 3 Semitones", -24, 24, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("OSC_3_DETUNE", "Oscilator 3 Detune", -50.0f, 50.0f, 0));
    //==================================================== NOISE OSC PARAMETERS ===================================================
    params.add(std::make_unique<juce::AudioParameterFloat>("NOISE_LEVEL", "Noise Level", 0.0f, 1.0f, 0.0f));
    //==================================================== MONOPHONIC PARAMETER ===================================================
    params.add(std::make_unique<juce::AudioParameterBool>("MONOPHONIC", "Monophonic", false));
    //==================================================== FILTER PARAMETERS ===================================================   
    params.add(std::make_unique<juce::AudioParameterChoice>("FILTER_CHOICE", "Filter", filterChoices, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", juce::NormalisableRange<float>(20.0f, 20000.f, 0.001f, 0.18f), 20000.0f));
    


    params.add(std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance", 1 / sqrt(2),10.0f, 1/sqrt(2)));
    params.add(std::make_unique<juce::AudioParameterChoice>("FILTER_MODE_CHOICE", "Filter Mode", filterModeChoices, 0));
    //==================================================== FILTER ADSR PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_ATTACK", "Filter Attack", 0.02f, 2.0f, 0.02f));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_DECAY", "Filter Decay", 0.0005f, 2.0f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_SUSTAIN", "Filter Sustain", 0.0f, 1.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_RELEASE", "Filter Release", 0.02f, 2.0f, 0.02f));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_ADSR_AMPLITUDE" , "Filter ADSR Amplitude" , juce::NormalisableRange<float>(20.0f, 20000.0f, 0.001f, 0.18f), 20000.0f));
    //==================================================== FILTER LFO PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterChoice>("FILTER_LFO_WAVEFORM_CHOICE", "Filter Lfo Waveform", filterLFOWaveformChoices, 0));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_LFO_RATE", "Filter Lfo Rate", 0.5f, 20.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_LFO_DEPTH", "Filter Lfo Depth", 0.0f, 1.0f, 0.5f));
    //==================================================== EFFECT CHOICE PARAMETER =================================================== 
    params.add(std::make_unique<juce::AudioParameterChoice>("EFFECT_CHOICE", "Effect Choice", effectChoices, 0));
    //==================================================== CHORUS PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterFloat>("CHORUS_CENTRE_DELAY", "Chorus Centre Delay", 1.0f, 100.0f, 10.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("CHORUS_RATE", "Chorus Rate", 0.1f, 20.0f, 10.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("CHORUS_DEPTH", "Chorus Depth", juce::NormalisableRange<float>(0.0f,1.0f,0.0001f,0.5f,false), 0.01f));
    params.add(std::make_unique<juce::AudioParameterFloat>("CHORUS_FEEDBACK", "Chorus Feedback", -1.0f, 1.0f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("CHORUS_MIX", "Chorus Mix", 0.0f, 1.0f, 0.0f));
    //==================================================== PHASER PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterFloat>("PHASER_CENTRE_FREQUENCY", "Phaser Centre Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 0.001f, 0.18f), 400.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("PHASER_RATE", "Phaser Rate", 0.1f, 20.0f, 10.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("PHASER_DEPTH", "Phaser Depth", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 0.5f, false), 0.01f));
    params.add(std::make_unique<juce::AudioParameterFloat>("PHASER_FEEDBACK", "Phaser Feedback", -1.0f, 1.0f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("PHASER_MIX", "Phaser Mix", 0.0f, 1.0f, 0.0f));
    //==================================================== REVERB PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_ROOM_SIZE", "Reverb Room Size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f, false), 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_WIDTH", "Reverb Width", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f, false), 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_DAMPING", "Reverb Damping", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f, false), 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_DRY", "Reverb Dry", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f, false), 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_WET", "Reverb Wet", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f, false), 0.0f));
    //==================================================== TUNING PARAMETERS ===================================================  
    params.add(std::make_unique<juce::AudioParameterChoice>("ROOT_NOTE_CHOICE", "Root Note Choice", rootNoteChoices, 0));

    params.add(std::make_unique<juce::AudioParameterInt>("ROOT", "Root", -100 , 100, 0.0f));
    params.add(std::make_unique<juce::AudioParameterInt>("MINOR_SECOND", "Minor Second", 0, 1200, 100));
    params.add(std::make_unique<juce::AudioParameterInt>("MAJOR_SECOND", "Major Second", 0, 1200, 200));
    params.add(std::make_unique<juce::AudioParameterInt>("MINOR_THIRD", "Minor Third", 0, 1200, 300));
    params.add(std::make_unique<juce::AudioParameterInt>("MAJOR_THIRD", "Major Third", 0, 1200, 400));
    params.add(std::make_unique<juce::AudioParameterInt>("PERFECT_FOURTH", "Perfect Fourth", 0, 1200, 500));
    params.add(std::make_unique<juce::AudioParameterInt>("DIMINISHED_FIFTH", "Diminished Fifth", 0, 1200, 600));
    params.add(std::make_unique<juce::AudioParameterInt>("PERFECT_FIFTH", "Perfect Fifth", 0, 1200, 700));
    params.add(std::make_unique<juce::AudioParameterInt>("MINOR_SIXTH", "Minor Sixth", 0, 1200, 800));
    params.add(std::make_unique<juce::AudioParameterInt>("MAJOR_SIXTH", "Major Sixth", 0, 1200, 900));
    params.add(std::make_unique<juce::AudioParameterInt>("MINOR_SEVENTH", "Minor Seventh", 0, 1200, 1000));
    params.add(std::make_unique<juce::AudioParameterInt>("MAJOR_SEVENTH", "Major Seventh", 0, 1200, 1100));

    params.add(std::make_unique<juce::AudioParameterBool>("ROOT_IS_ENABLED", "Root Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MINOR_SECOND_IS_ENABLED", "Minor Second Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MAJOR_SECOND_IS_ENABLED", "Major Second Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MINOR_THIRD_IS_ENABLED", "Minor Third Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MAJOR_THIRD_IS_ENABLED", "Major Third Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("PERFECT_FOURTH_IS_ENABLED", "Perfect Fourth Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("DIMINISHED_FIFTH_IS_ENABLED", "Diminished Fifth Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("PERFECT_FIFTH_IS_ENABLED", "Perfect Fifth Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MINOR_SIXTH_IS_ENABLED", "Minor Sixth Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MAJOR_SIXTH_IS_ENABLED", "Major Sixth Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MINOR_SEVENTH_IS_ENABLED", "Minor Seventh Is Enabled", true));
    params.add(std::make_unique<juce::AudioParameterBool>("MAJOR_SEVENTH_IS_ENABLED", "Major Seventh Is Enabled", true));

    params.add(std::make_unique<juce::AudioParameterChoice>("TUNING_CHOICE", "Tuning Choice", tuningChoices, 0));
    //=======================================================================================================================================================

    return params;
    
}


//======================================================================================================================================
int MPEVstProjectAudioProcessor::getNumActiveVoices() {
    return mpeAudioEngine.getNumActiveVoices();
}

//====================================== PARAMETER SETTING ============================================================================================================

//====================== MASTER VOLUME SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setMasterVolume(){
    masterVolume.setGainLinear(apvts.getRawParameterValue("MASTER")->load());
}
//====================== ADSR SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setVoiceAttack() {
    mpeAudioEngine.setVoiceAttack(apvts.getRawParameterValue("VOICE_ATTACK")->load());
}
void MPEVstProjectAudioProcessor::setVoiceDecay() {
    mpeAudioEngine.setVoiceDecay(apvts.getRawParameterValue("VOICE_DECAY")->load());
}
void MPEVstProjectAudioProcessor::setVoiceSustain() {
    mpeAudioEngine.setVoiceSustain(apvts.getRawParameterValue("VOICE_SUSTAIN")->load());
}
void MPEVstProjectAudioProcessor::setVoiceRelease() {
    mpeAudioEngine.setVoiceRelease(apvts.getRawParameterValue("VOICE_RELEASE")->load());
}
//====================== OSC 1 SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setOsc1() {
    mpeAudioEngine.setOsc1((int)apvts.getRawParameterValue("OSC_1")->load());
}
void MPEVstProjectAudioProcessor::setOsc1Level() {
    mpeAudioEngine.setOsc1Level(apvts.getRawParameterValue("OSC_1_LEVEL")->load());
}
void MPEVstProjectAudioProcessor::setOsc1Semitones() {
    mpeAudioEngine.setOsc1Semitones((int)apvts.getRawParameterValue("OSC_1_SEMITONES")->load());
}
void MPEVstProjectAudioProcessor::setOsc1Detune() {
    mpeAudioEngine.setOsc1Detune(apvts.getRawParameterValue("OSC_1_DETUNE")->load());
}
//====================== OSC 2 SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setOsc2() {
    mpeAudioEngine.setOsc2((int)apvts.getRawParameterValue("OSC_2")->load());
}
void MPEVstProjectAudioProcessor::setOsc2Level() {
    mpeAudioEngine.setOsc2Level(apvts.getRawParameterValue("OSC_2_LEVEL")->load());
}
void MPEVstProjectAudioProcessor::setOsc2Semitones() {
    mpeAudioEngine.setOsc2Semitones((int)apvts.getRawParameterValue("OSC_2_SEMITONES")->load());
}
void MPEVstProjectAudioProcessor::setOsc2Detune() {
    mpeAudioEngine.setOsc2Detune(apvts.getRawParameterValue("OSC_2_DETUNE")->load());
}
//====================== OSC 3 SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setOsc3() {
    mpeAudioEngine.setOsc3((int)apvts.getRawParameterValue("OSC_3")->load());
}
void MPEVstProjectAudioProcessor::setOsc3Level() {
    mpeAudioEngine.setOsc3Level(apvts.getRawParameterValue("OSC_3_LEVEL")->load());
}
void MPEVstProjectAudioProcessor::setOsc3Semitones() {
    mpeAudioEngine.setOsc3Semitones((int)apvts.getRawParameterValue("OSC_3_SEMITONES")->load());
}
void MPEVstProjectAudioProcessor::setOsc3Detune() {
    mpeAudioEngine.setOsc3Detune(apvts.getRawParameterValue("OSC_3_DETUNE")->load());
}
//====================== NOISE OSC SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setNoiseLevel() {
    mpeAudioEngine.setNoiseLevel(apvts.getRawParameterValue("NOISE_LEVEL")->load());
}
//====================== NUMBER OF VOICES SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setNumOfVoices() {
    mpeAudioEngine.setNumOfVoices((bool)apvts.getRawParameterValue("MONOPHONIC")->load(),{ sampleRate,(juce::uint32)samplesPerBlock, (juce::uint32)getTotalNumOutputChannels() }, &apvts);
}
//====================== FILTER SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setFilterCutoff() {
    mpeAudioEngine.setFilterCutoff(apvts.getRawParameterValue("CUTOFF")->load());
}
void MPEVstProjectAudioProcessor::setFilterResonance() {
    mpeAudioEngine.setFilterResonance(apvts.getRawParameterValue("RESONANCE")->load());
}
void MPEVstProjectAudioProcessor::setFilterType() {
    mpeAudioEngine.setFilterType((int)apvts.getRawParameterValue("FILTER_CHOICE")->load());
}
void MPEVstProjectAudioProcessor::setFilterMode() {
    mpeAudioEngine.setFilterMode((int)apvts.getRawParameterValue("FILTER_MODE_CHOICE")->load());
}
//====================== FILTER ADSR SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setFilterAttack() {
    mpeAudioEngine.setFilterAttack(apvts.getRawParameterValue("FILTER_ATTACK")->load());
}
void MPEVstProjectAudioProcessor::setFilterDecay() {
    mpeAudioEngine.setFilterDecay(apvts.getRawParameterValue("FILTER_DECAY")->load());
}
void MPEVstProjectAudioProcessor::setFilterSustain() {
    mpeAudioEngine.setFilterSustain(apvts.getRawParameterValue("FILTER_SUSTAIN")->load());
}
void MPEVstProjectAudioProcessor::setFilterRelease() {
    mpeAudioEngine.setFilterRelease(apvts.getRawParameterValue("FILTER_RELEASE")->load());
}
void MPEVstProjectAudioProcessor::setFilterADSRAmplitude() {
    mpeAudioEngine.setFilterADSRAmplitude(apvts.getRawParameterValue("FILTER_ADSR_AMPLITUDE")->load());
}
//====================== FILTER LFO SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setFilterLFOOsc() {
    mpeAudioEngine.setFilterLFOOsc((int)apvts.getRawParameterValue("FILTER_LFO_WAVEFORM_CHOICE")->load());
}
void MPEVstProjectAudioProcessor::setFilterLFORate() {
    mpeAudioEngine.setFilterLFORate(apvts.getRawParameterValue("FILTER_LFO_RATE")->load());
}
void MPEVstProjectAudioProcessor::setFilterLFODepth() {
    mpeAudioEngine.setFilterLFODepth(apvts.getRawParameterValue("FILTER_LFO_DEPTH")->load());
}
//====================== CHORUS SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setChorusCentreDelay() {
    mpeAudioEngine.setChorusCentreDelay(apvts.getRawParameterValue("CHORUS_CENTRE_DELAY")->load());
}
void MPEVstProjectAudioProcessor::setChorusDepth() {
    mpeAudioEngine.setChorusDepth(apvts.getRawParameterValue("CHORUS_DEPTH")->load());
}
void MPEVstProjectAudioProcessor::setChorusFeedback() {
    mpeAudioEngine.setChorusFeedback(apvts.getRawParameterValue("CHORUS_FEEDBACK")->load());
}
void MPEVstProjectAudioProcessor::setChorusMix() {
    mpeAudioEngine.setChorusMix(apvts.getRawParameterValue("CHORUS_MIX")->load());
}
void MPEVstProjectAudioProcessor::setChorusRate() {
    mpeAudioEngine.setChorusRate(apvts.getRawParameterValue("CHORUS_RATE")->load());
}
//====================== PHASER SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setPhaserCentreFrequency(){
    mpeAudioEngine.setPhaserCentreFrequency(apvts.getRawParameterValue("PHASER_CENTRE_FREQUENCY")->load());
}
void MPEVstProjectAudioProcessor::setPhaserDepth() {
    mpeAudioEngine.setPhaserDepth(apvts.getRawParameterValue("PHASER_DEPTH")->load());
}
void MPEVstProjectAudioProcessor::setPhaserRate() {
    mpeAudioEngine.setPhaserRate(apvts.getRawParameterValue("PHASER_RATE")->load());
}
void MPEVstProjectAudioProcessor::setPhaserFeedback() {
    mpeAudioEngine.setPhaserFeedback(apvts.getRawParameterValue("PHASER_FEEDBACK")->load());
}
void MPEVstProjectAudioProcessor::setPhaserMix() {
    mpeAudioEngine.setPhaserMix(apvts.getRawParameterValue("PHASER_MIX")->load());
}
//====================== REVERB SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setReverbDamping() {
    mpeAudioEngine.setReverbDamping(apvts.getRawParameterValue("REVERB_DAMPING")->load());
}
void MPEVstProjectAudioProcessor::setReverbRoomSize() {
    mpeAudioEngine.setReverbRoomSize(apvts.getRawParameterValue("REVERB_ROOM_SIZE")->load());
}
void MPEVstProjectAudioProcessor::setReverbWidth() {
    mpeAudioEngine.setReverbWidth(apvts.getRawParameterValue("REVERB_WIDTH")->load());
}
void MPEVstProjectAudioProcessor::setReverbWet() {
    mpeAudioEngine.setReverbWet(apvts.getRawParameterValue("REVERB_WET")->load());
}
void MPEVstProjectAudioProcessor::setReverbDry() {
    mpeAudioEngine.setReverbDry(apvts.getRawParameterValue("REVERB_DRY")->load());
}
//====================== TUNING SETTINGS ===================================================
void MPEVstProjectAudioProcessor::setRootNoteChoice() {
    mpeAudioEngine.setRootNoteChoice((int)apvts.getRawParameterValue("ROOT_NOTE_CHOICE")->load());
}

void MPEVstProjectAudioProcessor::setRootCents() {
    mpeAudioEngine.setRootCents((int)apvts.getRawParameterValue("ROOT")->load());
}
void MPEVstProjectAudioProcessor::setMinorSecondCents() {
    mpeAudioEngine.setMinorSecondCents((int)apvts.getRawParameterValue("MINOR_SECOND")->load());
}
void MPEVstProjectAudioProcessor::setMajorSecondCents() {
    mpeAudioEngine.setMajorSecondCents((int)apvts.getRawParameterValue("MAJOR_SECOND")->load());
}
void MPEVstProjectAudioProcessor::setMinorThirdCents() {
    mpeAudioEngine.setMinorThirdCents((int)apvts.getRawParameterValue("MINOR_THIRD")->load());
}
void MPEVstProjectAudioProcessor::setMajorThirdCents() {
    mpeAudioEngine.setMajorThirdCents((int)apvts.getRawParameterValue("MAJOR_THIRD")->load());
}
void MPEVstProjectAudioProcessor::setPerfectFourthCents() {
    mpeAudioEngine.setPerfectFourthCents((int)apvts.getRawParameterValue("PERFECT_FOURTH")->load());
}
void MPEVstProjectAudioProcessor::setDiminishedFifthCents() {
    mpeAudioEngine.setDiminishedFifthCents((int)apvts.getRawParameterValue("DIMINISHED_FIFTH")->load());
}
void MPEVstProjectAudioProcessor::setPerfectFifthCents() {
    mpeAudioEngine.setPerfectFifthCents((int)apvts.getRawParameterValue("PERFECT_FIFTH")->load());
}
void MPEVstProjectAudioProcessor::setMinorSixthCents() {
    mpeAudioEngine.setMinorSixthCents((int)apvts.getRawParameterValue("MINOR_SIXTH")->load());
}
void MPEVstProjectAudioProcessor::setMajorSixthCents() {
    mpeAudioEngine.setMajorSixthCents((int)apvts.getRawParameterValue("MAJOR_SIXTH")->load());
}
void MPEVstProjectAudioProcessor::setMinorSeventhCents() {
    mpeAudioEngine.setMinorSeventhCents((int)apvts.getRawParameterValue("MINOR_SEVENTH")->load());
}
void MPEVstProjectAudioProcessor::setMajorSeventhCents() {
    mpeAudioEngine.setMajorSeventhCents((int)apvts.getRawParameterValue("MAJOR_SEVENTH")->load());
}

void MPEVstProjectAudioProcessor::setRootIsEnabled() {
    mpeAudioEngine.setRootIsEnabled(apvts.getRawParameterValue("ROOT_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMinorSecondIsEnabled() {
    mpeAudioEngine.setMinorSecondIsEnabled(apvts.getRawParameterValue("MINOR_SECOND_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMajorSecondIsEnabled() {
    mpeAudioEngine.setMajorSecondIsEnabled(apvts.getRawParameterValue("MAJOR_SECOND_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMinorThirdIsEnabled() {
    mpeAudioEngine.setMinorThirdIsEnabled(apvts.getRawParameterValue("MINOR_THIRD_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMajorThirdIsEnabled() {
    mpeAudioEngine.setMajorThirdIsEnabled(apvts.getRawParameterValue("MAJOR_THIRD_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setPerfectFourthIsEnabled() {
    mpeAudioEngine.setPerfectFourthIsEnabled(apvts.getRawParameterValue("PERFECT_FOURTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setDiminishedFifthIsEnabled() {
    mpeAudioEngine.setDiminishedFifthIsEnabled(apvts.getRawParameterValue("DIMINISHED_FIFTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setPerfectFifthIsEnabled() {
    mpeAudioEngine.setPerfectFifthIsEnabled(apvts.getRawParameterValue("PERFECT_FIFTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMinorSixthIsEnabled() {
    mpeAudioEngine.setMinorSixthIsEnabled(apvts.getRawParameterValue("MINOR_SIXTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMajorSixthIsEnabled() {
    mpeAudioEngine.setMajorSixthIsEnabled(apvts.getRawParameterValue("MAJOR_SIXTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMinorSeventhIsEnabled() {
    mpeAudioEngine.setMinorSeventhIsEnabled(apvts.getRawParameterValue("MINOR_SEVENTH_IS_ENABLED")->load());
}
void MPEVstProjectAudioProcessor::setMajorSeventhIsEnabled() {
    mpeAudioEngine.setMajorSeventhIsEnabled(apvts.getRawParameterValue("MAJOR_SEVENTH_IS_ENABLED")->load());
}
//=======================================================================================================================



