/*
  ==============================================================================

    CustomOscillator.h
    Created: 12 Dec 2020 5:28:45am
    Author:  Sisma

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


class CustomOscillator : public juce::dsp::ProcessorBase
{
public:
    //==============================================================================
    CustomOscillator()
    {
        processorChain.template get<levelIndex>().setRampDurationSeconds(0.002f);

    }
    
    //===============================================================================
    ~CustomOscillator() {

    }
    //===========================================================================
    enum class Waveform
    {
        disabled,
        sine,
        saw,
        square,
        triangle,
        triangleFromSinHarmonics,
        squareFromSinHarmonics,
        sawFromSinHarmonics,
        squareTriangle,
        saw2,
        sawInPhase,
        triangleReal,
        smoothSquare,
        filteredSquare,
        organ,
        lofiTriangle,
        reallyLofiTriangle
    };
    
    void setWaveform(Waveform waveform) {

        auto pi = juce::MathConstants<float>::pi;
        
        switch (waveform)
        {
        case Waveform::disabled:
            processorChain.template get<oscIndex>().initialise([](float x) {
                return 0;
                }, oscLookupTableRes);
            break;
        case Waveform::sine:
            processorChain.template get<oscIndex>().initialise([](float x) {
                return std::sin(x);
                }, oscLookupTableRes);
            break;
        case Waveform::saw:
            processorChain.template get<oscIndex>().initialise([](float x) {
                return juce::jmap(x, float(-juce::MathConstants<double>::pi), float(juce::MathConstants<double>::pi), float(-1), float(1));
                }, oscLookupTableRes);
            break;
        case Waveform::square:          
            processorChain.template get<oscIndex>().initialise([](float x) {
                return std::sin(x) > 0 ? 1 : -1;
                }, oscLookupTableRes);
            break;
        case Waveform::triangle:
            processorChain.template get<oscIndex>().initialise([](float x) {
                return std::tan(std::sin(x));
                }, oscLookupTableRes);
            break;
        case Waveform::triangleFromSinHarmonics:
            processorChain.template get<oscIndex>().initialise([](float x) {

                int harmonics[] = { 1,3,5,7,9,11 };
                float harmonicWeights[] = { 1.0f,0.111f,0.04f,0.02f,0.012f,0.008f };

                float sample = 0;
                for (auto harmonic = 0; harmonic < juce::numElementsInArray(harmonics); harmonic++)
                {
                    sample = sample + std::sin(x * harmonics[harmonic]) * harmonicWeights[harmonic];
                }
                return sample;

                }, oscLookupTableRes);
            break;
        case Waveform::squareFromSinHarmonics:
            processorChain.template get<oscIndex>().initialise([](float x) {

                float sample = 0;
                float level = 0;
                for (int harmonic = 1; harmonic < 20; harmonic += 2)
                {
                    level = 1.0f / harmonic;
                    sample = sample + std::sin(x * harmonic) * level;
                }
                return sample;

                }, oscLookupTableRes);
            break;
        case Waveform::sawFromSinHarmonics:
            processorChain.template get<oscIndex>().initialise([](float x) {

                float sample = 0;
                float level = 0;
                for (int harmonic = 1; harmonic < 15; harmonic++)
                {
                    level = 1.0f / harmonic;
                    sample = sample + std::sin(x * harmonic) * level;
                }
                return sample;

                }, oscLookupTableRes);
            break;
        case Waveform::squareTriangle:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return -1 * (((x / pi >= 0) - (x / pi < 0)) * 0.5) + (std::abs(x / pi) - 0.5);
                }, oscLookupTableRes);
            break;
        case Waveform::saw2:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return x / pi;
                }, oscLookupTableRes);
            break;
        case Waveform::sawInPhase:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return x / pi < 0 ? -1 - x / pi : 1 - x / pi;
                }, oscLookupTableRes);
            break;
        case Waveform::triangleReal:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return std::min(2 + 2 * x / pi, (float)(abs((x / pi - 0.5) * 2) - 1));
                }, oscLookupTableRes);
            break;
        case Waveform::smoothSquare:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return juce::jlimit(-1.0f, 1.0f, ((std::abs(x / pi * 2) - 1) * 4 * pi));
                }, oscLookupTableRes);
            break;
        case Waveform::filteredSquare:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return 1.2 * std::sin(x) + 0.31 * std::sin(x * 3) + 0.11 * std::sin(x * 5) + 0.033 * std::sin(x * 7);
                }, oscLookupTableRes);
            break;
        case Waveform::organ:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return std::sin(x) + (0.16 * (std::sin(2 * x) + std::sin(3 * x) + std::sin(4 * x)));
                }, oscLookupTableRes);
            break;
        case Waveform::lofiTriangle:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return (std::rint(16 * std::abs(x / pi)) / 8.0) - 1;
                }, oscLookupTableRes);
            break;
        case Waveform::reallyLofiTriangle:
            processorChain.template get<oscIndex>().initialise([pi](float x) {
                return (std::rint(3 * std::abs(x / pi)) / 1.5) - 1;
                }, oscLookupTableRes);
            break;
        default:
            break;
        }
    }

    //==============================================================================
    void setFrequency(float newValue, bool force = false)
    {
        auto& osc = processorChain.template get<oscIndex>();
        osc.setFrequency(newValue, force);   
    }

    //==============================================================================
    void setVelocity(float newValue)
    {
        auto& gain = processorChain.template get<velocityIndex>();
        gain.setGainLinear(newValue);
        
    }
    //==============================================================================
    void setLevel(float newValue)
    {
        processorChain.template get<levelIndex>().setGainLinear(newValue);
    }

    //==============================================================================
    void reset() noexcept override
    {
        processorChain.reset();
    }

    //==============================================================================
    
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept override
    {
        processorChain.process(context);
    }

    //==============================================================================
    void prepare(const juce::dsp::ProcessSpec& spec) override
    {
        processorChain.prepare(spec);


    }

private:
    //==============================================================================
    enum
    {
        oscIndex,
        velocityIndex,
        levelIndex
    };


    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>,juce::dsp::Gain<float>, juce::dsp::Gain<float>> processorChain;

    int oscLookupTableRes = 256;

    
};
