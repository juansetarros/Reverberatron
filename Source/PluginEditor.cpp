/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverberatronAudioProcessorEditor::ReverberatronAudioProcessorEditor(ReverberatronAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    fondo = juce::ImageFileFormat::loadFrom(BinaryData::Reverberatron_jpg, BinaryData::Reverberatron_jpgSize);
    knobImagen = juce::ImageFileFormat::loadFrom(BinaryData::ReverberatronKnob_png, BinaryData::ReverberatronKnob_pngSize);
    sliderImagen = juce::ImageFileFormat::loadFrom(BinaryData::ReverberatronSlider_png, BinaryData::ReverberatronSlider_pngSize);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    knob1.reset(new ImageKnob(knobImagen, 31, true));
    addAndMakeVisible(knob1.get());
    knob1->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob1->setRange(0.5, 2.5);
    knob1->onValueChange = [this]
    {
        audioProcessor.Rt = knob1->getValue();
    };
    addAndMakeVisible(RTLabel);
    RTLabel.setText("RT", juce::dontSendNotification);
    RTLabel.setInterceptsMouseClicks(false, false);

    knob2.reset(new ImageKnob(knobImagen, 31, true));
    addAndMakeVisible(knob2.get());
    knob2->setRange(1000, 10000);
    knob2->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob2->setSkewFactorFromMidPoint(5000);
    //knob2->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    knob2->onValueChange = [this]
    {
        audioProcessor.Vr = knob2->getValue();
    };
    addAndMakeVisible(VolumeLabel);
    VolumeLabel.setText("Volume of the Room", juce::dontSendNotification);
    VolumeLabel.setInterceptsMouseClicks(false, false);

    knob3.reset(new ImageKnob(knobImagen, 31, true));
    addAndMakeVisible(knob3.get());
    knob3->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob3->setRange(1000, 20000);
    knob3->setSkewFactorFromMidPoint(4000);
    knob3->onValueChange = [this]
    {
        audioProcessor.LPF.changeFc(knob3->getValue());
    };
    addAndMakeVisible(FilterLabel);
    FilterLabel.setText("Lowpass", juce::dontSendNotification);
    FilterLabel.setInterceptsMouseClicks(false, false);

    slider1.reset(new ImageKnob(sliderImagen, 31, true));
    addAndMakeVisible(slider1.get());
    slider1->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider1->setRange(0, 1);
    slider1->setValue(1);
    slider1->onValueChange = [this]
    {
        audioProcessor.dw = slider1->getValue();
    };
    addAndMakeVisible(DryWetLabel);
    DryWetLabel.setText("Dry/Wet", juce::dontSendNotification);
    DryWetLabel.setInterceptsMouseClicks(false, false);

    setSize (400, 300);
}

ReverberatronAudioProcessorEditor::~ReverberatronAudioProcessorEditor()
{
}

//==============================================================================
void ReverberatronAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Rectangle<float> dimensiones = getBounds().toFloat();
    g.drawImage(fondo, dimensiones);
}

void ReverberatronAudioProcessorEditor::resized()
{
    knob1->setBounds(30, 40, 70, 70);
    RTLabel.setBounds(50, 75, 100, 100);

    knob2->setBounds(30, 160, 70, 70);
    VolumeLabel.setBounds(5, 195, 120, 100);
    
    knob3->setBounds(300, 110, 70, 70);
    FilterLabel.setBounds(305, 150, 100, 100);
    
    slider1->setBounds(185, 80, 32, 128);
    DryWetLabel.setBounds(170, 15, 100, 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
