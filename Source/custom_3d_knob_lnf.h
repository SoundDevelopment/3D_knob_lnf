/*
  =====================================================================================================

    custom_3d_knob_lnf.h
    Created  : 31 Jul 2022 10:12:31pm
    Author   : Marcel Huibers
    Project  : 3D_knob_lnf
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2022 All Rights Reserved

  =====================================================================================================
*/

#pragma once

#include <JuceHeader.h>

class Custom_3d_knob_lnf : public juce::LookAndFeel_V4
{
public:

    // clang-format off
    Custom_3d_knob_lnf();

    void drawRotarySlider( juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider ) override;

};
