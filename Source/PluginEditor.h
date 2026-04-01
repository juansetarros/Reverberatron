/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ImageKnob.h"

//==============================================================================
/**
*/
class ReverberatronAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverberatronAudioProcessorEditor (ReverberatronAudioProcessor&);
    ~ReverberatronAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Image fondo;
    juce::Image knobImagen;
    juce::Image sliderImagen;

    std::unique_ptr<ImageKnob> knob1;
    juce::Label RTLabel;
    std::unique_ptr<ImageKnob> knob2;
    juce::Label VolumeLabel;
    std::unique_ptr<ImageKnob> knob3;
    juce::Label FilterLabel;

    std::unique_ptr<ImageKnob> slider1;
    juce::Label DryWetLabel;


    ReverberatronAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverberatronAudioProcessorEditor)
};
