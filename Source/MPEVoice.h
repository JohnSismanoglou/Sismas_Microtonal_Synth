/*
  ==============================================================================

    MPEVoice.h
    Created: 12 Dec 2020 3:27:52am
    Author:  Sisma

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CustomOscillator.h"


class MPEVoice : public juce::MPESynthesiserVoice
{
public:
    MPEVoice()
    {

    }

    //==============================================================================


    void prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState* apvts)
    {
        //========================= PREPARING AUDIO BLOCKS ==================================================================
        tempBlock = juce::dsp::AudioBlock<float>(heapBlock, spec.numChannels, spec.maximumBlockSize);
        osc1TempBlock = juce::dsp::AudioBlock<float>(osc1HeapBlock, spec.numChannels, spec.maximumBlockSize);
        osc2TempBlock = juce::dsp::AudioBlock<float>(osc2HeapBlock, spec.numChannels, spec.maximumBlockSize);
        osc3TempBlock = juce::dsp::AudioBlock<float>(osc3HeapBlock, spec.numChannels, spec.maximumBlockSize);
        filterLfoTempBlock = juce::dsp::AudioBlock<float>(filterLfoHeapBlock, spec.numChannels, spec.maximumBlockSize);  

        //========================= PREPARING OSCS ==========================================================================
        osc1.prepare(spec);
        osc1SemiCents = (int)apvts->getRawParameterValue("OSC_1_SEMITONES")->load() * 100;
        osc1DetuneCents = apvts->getRawParameterValue("OSC_1_DETUNE")->load();
        osc1.setWaveform((CustomOscillator::Waveform)apvts->getRawParameterValue("OSC_1")->load());
        osc1.setLevel(apvts->getRawParameterValue("OSC_1_LEVEL")->load());

        osc2.prepare(spec);
        osc2SemiCents = (int)apvts->getRawParameterValue("OSC_2_SEMITONES")->load() * 100;
        osc2DetuneCents = apvts->getRawParameterValue("OSC_2_DETUNE")->load();
        osc2.setWaveform((CustomOscillator::Waveform)apvts->getRawParameterValue("OSC_2")->load());
        osc2.setLevel(apvts->getRawParameterValue("OSC_2_LEVEL")->load());

        osc3.prepare(spec);
        osc3SemiCents = (int)apvts->getRawParameterValue("OSC_3_SEMITONES")->load() * 100;
        osc3DetuneCents = apvts->getRawParameterValue("OSC_3_DETUNE")->load();
        osc3.setWaveform((CustomOscillator::Waveform)apvts->getRawParameterValue("OSC_3")->load());
        osc3.setLevel(apvts->getRawParameterValue("OSC_3_LEVEL")->load());

        noiseGain.prepare(spec);
        noiseGain.setRampDurationSeconds(0.2f);
        noiseGain.setGainLinear(apvts->getRawParameterValue("NOISE_LEVEL")->load());
        
        //========================= PREPARING VOICE ADSR ========================================================================== 
        adsrGain.prepare(spec);
        voiceADSR.setSampleRate(spec.sampleRate);

        voiceADSRParameters.attack = apvts->getRawParameterValue("VOICE_ATTACK")->load();
        voiceADSRParameters.decay = apvts->getRawParameterValue("VOICE_DECAY")->load();
        voiceADSRParameters.sustain = apvts->getRawParameterValue("VOICE_SUSTAIN")->load();
        voiceADSRParameters.release = apvts->getRawParameterValue("VOICE_RELEASE")->load();

        //========================= PREPARING FILTER ========================================================================== 
        cutoffFreq = apvts->getRawParameterValue("CUTOFF")->load();
        filterResonance = apvts->getRawParameterValue("RESONANCE")->load();

        filter.setCutoffFrequency(cutoffFreq);
        filter.setResonance(filterResonance);
        filter.setType((juce::dsp::StateVariableTPTFilterType)apvts->getRawParameterValue("FILTER_CHOICE")->load());

        filterModeChoice = (int)apvts->getRawParameterValue("FILTER_MODE_CHOICE")->load();

        filter.prepare(spec);

        //========================= PREPARING FILTER ADSR ========================================================================== 
        filterADSRParameters.attack = apvts->getRawParameterValue("FILTER_ATTACK")->load();
        filterADSRParameters.decay = apvts->getRawParameterValue("FILTER_DECAY")->load();
        filterADSRParameters.sustain = apvts->getRawParameterValue("FILTER_SUSTAIN")->load();
        filterADSRParameters.release = apvts->getRawParameterValue("FILTER_RELEASE")->load();
        filterADSRAmplitude = apvts->getRawParameterValue("FILTER_ADSR_AMPLITUDE")->load();

        filterADSR.setSampleRate(spec.sampleRate);

        //========================= PREPARING FILTER LFO ========================================================================== 
        setFilterLFOWaveform((LFOWaveform)apvts->getRawParameterValue("FILTER_LFO_WAVEFORM_CHOICE")->load());

        filterLFO.setFrequency(apvts->getRawParameterValue("FILTER_LFO_RATE")->load());
        filterLFOFreq = apvts->getRawParameterValue("FILTER_LFO_RATE")->load();
        filterLFODepth.setGainLinear(apvts->getRawParameterValue("FILTER_LFO_DEPTH")->load());
        filterLFODepth.setRampDurationSeconds(0.1f);

        //========================= PREPARING TUNING ========================================================================== 
        rootNoteChoice = (int)apvts->getRawParameterValue("ROOT_NOTE_CHOICE")->load();

        rootCents = (int)apvts->getRawParameterValue("ROOT")->load();
        minorSecondCents = (int)apvts->getRawParameterValue("MINOR_SECOND")->load();
        majorSecondCents = (int)apvts->getRawParameterValue("MAJOR_SECOND")->load();
        minorThirdCents = (int)apvts->getRawParameterValue("MINOR_THIRD")->load();
        majorThirdCents = (int)apvts->getRawParameterValue("MAJOR_THIRD")->load();
        perfectFourthCents = (int)apvts->getRawParameterValue("PERFECT_FOURTH")->load();
        diminishedFifthCents = (int)apvts->getRawParameterValue("DIMINISHED_FIFTH")->load();
        perfectFifthCents = (int)apvts->getRawParameterValue("PERFECT_FIFTH")->load();
        minorSixthCents = (int)apvts->getRawParameterValue("MINOR_SIXTH")->load();
        majorSixthCents = (int)apvts->getRawParameterValue("MAJOR_SIXTH")->load();
        minorSeventhCents = (int)apvts->getRawParameterValue("MINOR_SEVENTH")->load();
        majorSeventhCents = (int)apvts->getRawParameterValue("MAJOR_SEVENTH")->load();

        rootIsEnabled = apvts->getRawParameterValue("ROOT_IS_ENABLED")->load();
        minorSecondIsEnabled = apvts->getRawParameterValue("MINOR_SECOND_IS_ENABLED")->load();
        majorSecondIsEnabled = apvts->getRawParameterValue("MAJOR_SECOND_IS_ENABLED")->load();
        minorThirdIsEnabled = apvts->getRawParameterValue("MINOR_THIRD_IS_ENABLED")->load();
        majorThirdIsEnabled = apvts->getRawParameterValue("MAJOR_THIRD_IS_ENABLED")->load();
        perfectFourthIsEnabled = apvts->getRawParameterValue("PERFECT_FOURTH_IS_ENABLED")->load();
        diminishedFifthIsEnabled = apvts->getRawParameterValue("DIMINISHED_FIFTH_IS_ENABLED")->load();
        perfectFifthIsEnabled = apvts->getRawParameterValue("PERFECT_FIFTH_IS_ENABLED")->load();
        minorSixthIsEnabled = apvts->getRawParameterValue("MINOR_SIXTH_IS_ENABLED")->load();
        majorSixthIsEnabled = apvts->getRawParameterValue("MAJOR_SIXTH_IS_ENABLED")->load();
        minorSeventhIsEnabled = apvts->getRawParameterValue("MINOR_SEVENTH_IS_ENABLED")->load();
        majorSeventhIsEnabled = apvts->getRawParameterValue("MAJOR_SEVENTH_IS_ENABLED")->load();

    }

    //==============================================================================
    void noteStarted() override
    {

        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            clearCurrentNote();

        auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto cyclesPerSample = freqHz / getSampleRate();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        
        auto osc1Freq = std::pow((float)2, (float)((osc1SemiCents + osc1DetuneCents + tuningCents) / 1200)) * freqHz;
        osc1.setFrequency(osc1Freq, true);
        osc1.setVelocity(velocity);

        auto osc2Freq = std::pow((float)2, (float)((osc2SemiCents + osc2DetuneCents + tuningCents) / 1200)) * freqHz;
        osc2.setFrequency(osc2Freq, true);
        osc2.setVelocity(velocity);

        auto osc3Freq = std::pow((float)2, (float)((osc3SemiCents + osc3DetuneCents + tuningCents) / 1200)) * freqHz;
        osc3.setFrequency(osc3Freq, true);
        osc3.setVelocity(velocity);

        voiceADSR.setParameters(voiceADSRParameters);
        filterADSR.setParameters(filterADSRParameters);
        
        filterLFO.setFrequency(filterLFOFreq,true);
        voiceADSR.noteOn();
        filterADSR.noteOn();

    }

    //==============================================================================
    void notePitchbendChanged() override
    {
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
 
        auto osc1Freq = std::pow((float)2, (float)((osc1SemiCents + osc1DetuneCents + tuningCents) / 1200)) * freqHz;
        osc1.setFrequency(osc1Freq);

        auto osc2Freq = std::pow((float)2, (float)((osc2SemiCents + osc2DetuneCents + tuningCents) / 1200)) * freqHz;
        osc2.setFrequency(osc2Freq);

        auto osc3Freq = std::pow((float)2, (float)((osc3SemiCents + osc3DetuneCents + tuningCents) / 1200)) * freqHz;
        osc3.setFrequency(osc3Freq);

    }

    //==============================================================================
    void noteStopped(bool allowTailOff) override
    {

        if (allowTailOff) {
            voiceADSR.noteOff();
            filterADSR.noteOff();
        }    
        else
        {
            clearCurrentNote();
        }

    }

    //==============================================================================
    void noteForceStopped()
    {

        auto ADSR = voiceADSRParameters;
        auto filterADSRParams = filterADSRParameters;
        ADSR.release = 0.005f;
        filterADSRParams.release = 0.005f;
        voiceADSR.setParameters(ADSR);
        filterADSR.setParameters(filterADSRParams);
        voiceADSR.noteOff();
        filterADSR.noteOff();  

    }

    //==============================================================================
    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}

    //==============================================================================
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {     

        auto block = tempBlock.getSubBlock(0, numSamples);
        auto osc1Block = osc1TempBlock.getSubBlock(0, numSamples);
        auto osc2Block = osc2TempBlock.getSubBlock(0, numSamples);
        auto osc3Block = osc3TempBlock.getSubBlock(0, numSamples);
        auto filterLfoBlock = filterLfoTempBlock.getSubBlock(0, numSamples);

        block.clear();
        osc1Block.clear();
        osc2Block.clear();
        osc3Block.clear();
        filterLfoBlock.clear();

        juce::dsp::ProcessContextReplacing<float> context(block);
        juce::dsp::ProcessContextReplacing<float> osc1Context(osc1Block);
        juce::dsp::ProcessContextReplacing<float> osc2Context(osc2Block);
        juce::dsp::ProcessContextReplacing<float> osc3Context(osc3Block);
        juce::dsp::ProcessContextReplacing<float> filterLfoContext(filterLfoBlock);       

        osc1.process(osc1Context);
        osc2.process(osc2Context);
        osc3.process(osc3Context);
        filterLFO.process(filterLfoContext);
        filterLFODepth.process(filterLfoContext);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto noiceOsc = randomNoise.nextFloat() * 0.1f - 0.2f;
            noiceOsc = noiceOsc * noiseGain.getGainLinear();

            auto adsrSample = voiceADSR.getNextSample();
            adsrGain.setGainLinear(adsrSample);
            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                


                auto sumOfOsc = osc1Block.getSample(channel, sample) + osc2Block.getSample(channel, sample) + osc3Block.getSample(channel, sample) + noiceOsc;

                auto sampleToUse = filterProcesing(channel, sumOfOsc, (filterMode)filterModeChoice , filterLfoBlock.getSample(channel,sample));

                block.setSample(channel, sample, sampleToUse);
                block.setSample(channel, sample, adsrGain.processSample(block.getSample(channel, sample)));

            }

         
        }

        juce::dsp::AudioBlock<float>(outputBuffer).getSubBlock((size_t)startSample, (size_t)numSamples).add(block);

        if (!voiceADSR.isActive()) {
            clearCurrentNote();
            filter.setCutoffFrequency(cutoffFreq);
            filterADSR.reset();
            filterLFO.reset();         
        }

    }

//====================================== PARAMETER SETTING ============================================================================================================

    //====================== ADSR SETTINGS ===================================================
    void setVoiceAttack(float attackValue) {
        voiceADSRParameters.attack = attackValue;
        voiceADSR.setParameters(voiceADSRParameters);
    }
    void setVoiceDecay(float decayValue) {
        voiceADSRParameters.decay = decayValue;
        voiceADSR.setParameters(voiceADSRParameters);
    }
    void setVoiceSustain(float sustainValue) {
        voiceADSRParameters.sustain = sustainValue;
        voiceADSR.setParameters(voiceADSRParameters);
    }
    void setVoiceRelease(float releaseValue) {
        voiceADSRParameters.release = releaseValue;
        voiceADSR.setParameters(voiceADSRParameters);
    }
    //====================== OSC 1 SETTINGS ===================================================
    void setOsc1(int waveformChoice) {      
        osc1.setWaveform((CustomOscillator::Waveform)waveformChoice);
    }
    void setOsc1Level(float oscLevel) {
        osc1.setLevel(oscLevel);
    }
    void setOsc1Semitones(int oscSemitones) {
        osc1SemiCents = oscSemitones * 100;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc1Freq = std::pow((float)2, (float)((osc1SemiCents + osc1DetuneCents + tuningCents) / 1200)) * freqHz;
        osc1.setFrequency(osc1Freq, false);
    }
    void setOsc1Detune(float oscDetune) {
        osc1DetuneCents = oscDetune;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc1Freq = std::pow((float)2, (float)((osc1SemiCents + osc1DetuneCents + tuningCents) / 1200)) * freqHz;
        osc1.setFrequency(osc1Freq, false);
    }
    //====================== OSC 2 SETTINGS ===================================================
    void setOsc2(int waveformChoice) {      
        osc2.setWaveform((CustomOscillator::Waveform)waveformChoice);
    }
    void setOsc2Level(float oscLevel) {
        osc2.setLevel(oscLevel);
    }
    void setOsc2Semitones(int oscSemitones) {
        osc2SemiCents = oscSemitones * 100;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc2Freq = std::pow((float)2, (float)((osc2SemiCents + osc2DetuneCents + tuningCents) / 1200)) * freqHz;
        osc2.setFrequency(osc2Freq, false);
    }    
    void setOsc2Detune(float oscDetune) {
        osc2DetuneCents = oscDetune;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc2Freq = std::pow((float)2, (float)((osc2SemiCents + osc2DetuneCents + tuningCents) / 1200)) * freqHz;
        osc2.setFrequency(osc2Freq, false);
    }
    //====================== OSC 3 SETTINGS ===================================================
    void setOsc3(int waveformChoice) {
        osc3.setWaveform((CustomOscillator::Waveform)waveformChoice);
    }   
    void setOsc3Level(float oscLevel) {  
        osc3.setLevel(oscLevel);
    }
    void setOsc3Semitones(int oscSemitones) {
        osc3SemiCents = oscSemitones * 100;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc3Freq = std::pow((float)2, (float)((osc3SemiCents + osc3DetuneCents + tuningCents) / 1200)) * freqHz;
        osc3.setFrequency(osc3Freq, false);
    }
    void setOsc3Detune(float oscDetune) {
        osc3DetuneCents = oscDetune;
        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc3Freq = std::pow((float)2, (float)((osc3SemiCents + osc3DetuneCents + tuningCents) / 1200)) * freqHz;
        osc3.setFrequency(osc3Freq, false);
    }
    //====================== NOISE OSC SETTINGS ===================================================
    void setNoiseLevel(float noiseLevel) {
        noiseGain.setGainLinear(noiseLevel);
    }
    //====================== FILTER SETTINGS ===================================================
    void setFilterCutoff(float cutoff) {
        cutoffFreq = cutoff;
        filter.setCutoffFrequency(cutoffFreq);
    }
    void setFilterResonance(float resonance) {
        filterResonance = resonance;
        filter.setResonance(filterResonance);
    }
    void setFilterType(int filterTypeChoice) {
        filter.setType((juce::dsp::StateVariableTPTFilterType)filterTypeChoice);
    }
    void setFilterMode(int filterMode) {
        filterModeChoice = filterMode;
    }
    //====================== FILTER ADSR SETTINGS ===================================================
    void setFilterAttack(float attackValue) {
        filterADSRParameters.attack = attackValue;
        filterADSR.setParameters(filterADSRParameters);
    }
    void setFilterDecay(float decayValue) {
        filterADSRParameters.decay = decayValue;
        filterADSR.setParameters(filterADSRParameters);
    }
    void setFilterSustain(float sustainValue) {
        filterADSRParameters.sustain = sustainValue;
        filterADSR.setParameters(filterADSRParameters);
    }
    void setFilterRelease(float releaseValue) {
        filterADSRParameters.release = releaseValue;
        filterADSR.setParameters(filterADSRParameters);
    }
    void setFilterADSRAmplitude(float amplitudeValue) {
        filterADSRAmplitude = amplitudeValue;
    }
    //====================== FILTER LFO SETTINGS ===================================================
    void setFilterLFOOsc(int oscChoice) {
        setFilterLFOWaveform((LFOWaveform)oscChoice);
    }
    void setFilterLFORate(float rate) {
        filterLFOFreq = rate;
        filterLFO.setFrequency(rate,true);
    }
    void setFilterLFODepth(float depth) {
        filterLFODepth.setGainLinear(depth);
    }
    //====================== TUNING SETTINGS ===================================================
    void setRootNoteChoice(int rootNote) {
        rootNoteChoice = rootNote;
        setFrequencyAfterTuningCentsChange();
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }

    void setRootCents(int newRootCents) {
        rootCents = newRootCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMinorSecondCents(int newMinorSecondCents) {
        minorSecondCents = newMinorSecondCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMajorSecondCents(int newMajorSecondCents) {
        majorSecondCents = newMajorSecondCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMinorThirdCents(int newMinorThirdCents) {
        minorThirdCents = newMinorThirdCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMajorThirdCents(int newMajorThirdCents) {
        majorThirdCents = newMajorThirdCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setPerfectFourthCents(int newPerfectFourthCents) {
        perfectFourthCents = newPerfectFourthCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setDiminishedFifthCents(int newDiminishedFifthCents) {
        diminishedFifthCents = newDiminishedFifthCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setPerfectFifthCents(int newPerfectFifthCents) {
        perfectFifthCents = newPerfectFifthCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMinorSixthCents(int newMinorSixthCents) {
        minorSixthCents = newMinorSixthCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMajorSixthCents(int newMajorSixthCents) {
        majorSixthCents = newMajorSixthCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMinorSeventhCents(int newMinorSeventhCents) {
        minorSeventhCents = newMinorSeventhCents;
        setFrequencyAfterTuningCentsChange();
    }
    void setMajorSeventhCents(int newMajorSeventhCents) {
        majorSeventhCents = newMajorSeventhCents;
        setFrequencyAfterTuningCentsChange();
    }

    void setRootIsEnabled(bool isEnabled) {
        rootIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMinorSecondIsEnabled(bool isEnabled) {
        minorSecondIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMajorSecondIsEnabled(bool isEnabled) {
        majorSecondIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMinorThirdIsEnabled(bool isEnabled) {
        minorThirdIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMajorThirdIsEnabled(bool isEnabled) {
        majorThirdIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setPerfectFourthIsEnabled(bool isEnabled) {
        perfectFourthIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setDiminishedFifthIsEnabled(bool isEnabled) {
        diminishedFifthIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setPerfectFifthIsEnabled(bool isEnabled) {
        perfectFifthIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMinorSixthIsEnabled(bool isEnabled) {
        minorSixthIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMajorSixthIsEnabled(bool isEnabled) {
        majorSixthIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMinorSeventhIsEnabled(bool isEnabled) {
        minorSeventhIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }
    void setMajorSeventhIsEnabled(bool isEnabled) {
        majorSeventhIsEnabled = isEnabled;
        if (!noteIsEnabled(getCurrentlyPlayingNote().initialNote))
            noteForceStopped();
    }  
    //========================================================================================


private:

    //================== FILTER RELATED VARIABLES =============================================
       
    juce::dsp::Oscillator<float> filterLFO;
    juce::dsp::Gain<float> filterLFODepth;
    float filterLFOFreq; 

    int filterModeChoice;

    juce::dsp::StateVariableTPTFilter<float> filter;

    float cutoffFreq;
    float filterResonance;

    juce::ADSR filterADSR;
    juce::ADSR::Parameters filterADSRParameters;
    float filterADSRAmplitude;

    //================= OSCILATOR RELATED VARIABLES ============================================

    CustomOscillator osc1, osc2, osc3;
    juce::dsp::Gain<float> adsrGain;
    juce::dsp::Gain<float> noiseGain;
    juce::Random randomNoise;


    int osc1SemiCents;
    int osc2SemiCents;
    int osc3SemiCents;

    float osc1DetuneCents;
    float osc2DetuneCents;
    float osc3DetuneCents;

    juce::ADSR voiceADSR;
    juce::ADSR::Parameters voiceADSRParameters;

    //================= TUNING RELATED VARIABLES ==============================================

    int rootNoteChoice;

    int rootCents;
    int minorSecondCents;
    int majorSecondCents;
    int minorThirdCents;
    int majorThirdCents;
    int perfectFourthCents;
    int diminishedFifthCents;
    int perfectFifthCents;
    int minorSixthCents;
    int majorSixthCents;
    int minorSeventhCents;
    int majorSeventhCents;

    bool rootIsEnabled;
    bool minorSecondIsEnabled;
    bool majorSecondIsEnabled;
    bool minorThirdIsEnabled;
    bool majorThirdIsEnabled;
    bool perfectFourthIsEnabled;
    bool diminishedFifthIsEnabled;
    bool perfectFifthIsEnabled;
    bool minorSixthIsEnabled;
    bool majorSixthIsEnabled;
    bool minorSeventhIsEnabled;
    bool majorSeventhIsEnabled;

    //================== AUDIO BLOCKS ==================================================================

    juce::HeapBlock<char> heapBlock;
    juce::HeapBlock<char> osc1HeapBlock;
    juce::HeapBlock<char> osc2HeapBlock;
    juce::HeapBlock<char> osc3HeapBlock;
    juce::HeapBlock<char> filterLfoHeapBlock;

    juce::dsp::AudioBlock<float> tempBlock;
    juce::dsp::AudioBlock<float> osc1TempBlock;
    juce::dsp::AudioBlock<float> osc2TempBlock;
    juce::dsp::AudioBlock<float> osc3TempBlock;
    juce::dsp::AudioBlock<float> filterLfoTempBlock;
    
    //=============================================================================================================
    enum class LFOWaveform
    {
        disabled,
        sine,
        triangle,
        saw,
        sawInvert,
        square,
        squareInvert
    };

    void setFilterLFOWaveform(LFOWaveform waveform) {
        switch (waveform)
        {
        case LFOWaveform::disabled:
            filterLFO.initialise([](float x) {
                return 0;
                }, 128);
            break;
        case LFOWaveform::sine:
            filterLFO.initialise([](float x) {
                return std::sin(x);
                }, 128);
            break;
        case LFOWaveform::triangle:
            filterLFO.initialise([](float x) {
                return std::min(2 + 2 * x / juce::MathConstants<float>::pi, (float)(abs((x / juce::MathConstants<float>::pi - 0.5) * 2) - 1));
                }, 128);
            break;
        case LFOWaveform::saw:
            filterLFO.initialise([](float x) {
                return juce::jmap(x, float(-juce::MathConstants<double>::pi), float(juce::MathConstants<double>::pi), float(-1), float(1));
                }, 128);
            break;
        case LFOWaveform::sawInvert:
            filterLFO.initialise([](float x) {
                return juce::jmap(x, float(-juce::MathConstants<double>::pi), float(juce::MathConstants<double>::pi), float(-1), float(1)) * -1.0f;
                }, 128);
            break;
        case LFOWaveform::square:
            filterLFO.initialise([](float x) {
                return std::sin(x) > 0 ? 1 : -1;
                }, 128);
            break;
        case LFOWaveform::squareInvert:
            filterLFO.initialise([](float x) {
                return std::sin(x) > 0 ? -1 : 1;
                }, 128);
            break;
        }
    }
    //=============================================================================================================
    int noteTuningCents(int currentNoteNumber) {

        auto currentNote = (currentNoteNumber - rootNoteChoice) % 12;


        switch (currentNote)
        {
        case 0:
            return rootCents;
        case 1:
            return rootCents + minorSecondCents - 100;
        case 2:
            return rootCents + majorSecondCents - 200;
        case 3:
            return rootCents + minorThirdCents - 300;
        case 4:
            return rootCents + majorThirdCents - 400;
        case 5:
            return rootCents + perfectFourthCents - 500;
        case 6:
            return rootCents + diminishedFifthCents - 600;
        case 7:
            return rootCents + perfectFifthCents - 700;
        case 8:
            return rootCents + minorSixthCents - 800;
        case 9:
            return rootCents + majorSixthCents - 900;
        case 10:
            return rootCents + minorSeventhCents - 1000;
        case 11:
            return rootCents + majorSeventhCents - 1100;
        default:
            return rootCents;
            break;
        }


    }
    //=============================================================================================================
    bool noteIsEnabled(int currentNoteNumber) {

        auto currentNote = (currentNoteNumber - rootNoteChoice) % 12;

        switch (currentNote)
        {
        case 0:
            return rootIsEnabled;
            break;
        case 1:
            return minorSecondIsEnabled;
            break;
        case 2:
            return majorSecondIsEnabled;
            break;
        case 3:
            return minorThirdIsEnabled;
            break;
        case 4:
            return majorThirdIsEnabled;
            break;
        case 5:
            return perfectFourthIsEnabled;
            break;
        case 6:
            return diminishedFifthIsEnabled;
            break;
        case 7:
            return perfectFifthIsEnabled;
            break;
        case 8:
            return minorSixthIsEnabled;
            break;
        case 9:
            return majorSixthIsEnabled;
            break;
        case 10:
            return minorSeventhIsEnabled;
            break;
        case 11:
            return majorSeventhIsEnabled;
            break;
        default:
            return true;
            break;
        }
    }
    //=============================================================================================================
    void setFrequencyAfterTuningCentsChange() {

        auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
        auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
        auto osc1Freq = std::pow((float)2, (float)((osc1SemiCents + osc1DetuneCents + tuningCents) / 1200)) * freqHz;
        auto osc2Freq = std::pow((float)2, (float)((osc2SemiCents + osc2DetuneCents + tuningCents) / 1200)) * freqHz;
        auto osc3Freq = std::pow((float)2, (float)((osc3SemiCents + osc3DetuneCents + tuningCents) / 1200)) * freqHz;
        osc1.setFrequency(osc1Freq, false);
        osc2.setFrequency(osc2Freq, false);
        osc3.setFrequency(osc3Freq, false);

    }
    //=============================================================================================================
    enum class filterMode
    {
        staticFilter,
        ADSRFilter,
        keyboardTrackedFilter,
        LFOFilter
    };

    float filterProcesing(int channel, float sample, filterMode mode, float lfoSample) {

        switch (mode)
        {
        case filterMode::staticFilter:
        {
            return filter.processSample(channel, sample);
            break;
        }
        case filterMode::ADSRFilter:
        {
            float filterAdsrSample = filterADSR.getNextSample();
            if (filterADSRAmplitude > cutoffFreq)
                filter.setCutoffFrequency(juce::jmap(filterAdsrSample, cutoffFreq, filterADSRAmplitude));
            else
                filter.setCutoffFrequency(cutoffFreq);
            return filter.processSample(channel, sample);
            break;
        }
        case filterMode::keyboardTrackedFilter:
        {
            auto tuningCents = noteTuningCents(getCurrentlyPlayingNote().initialNote);
            float keyboardCents = 1200 * std::log2f(cutoffFreq / 220.0f);
            float keyboardCutoff = std::pow((float)2, (float)((keyboardCents + tuningCents) / 1200)) * (float)currentlyPlayingNote.getFrequencyInHertz();
            filter.setCutoffFrequency(keyboardCutoff > 20000.0f ? 20000.0f : keyboardCutoff);
            return filter.processSample(channel, sample);
            break;
        }
        case filterMode::LFOFilter:
        {
            float lfoCutoff = juce::jmap(lfoSample, -1.0f, 1.0f, cutoffFreq / 4, cutoffFreq * 4);
            filter.setCutoffFrequency(lfoCutoff > 20000.0f ? 20000.0f : lfoCutoff);
            return filter.processSample(channel, sample);
            break;
        }
        default:
            return sample;
        }

    }

};
