/*
  =====================================================================================================

    custom_3d_knob_lnf.cpp
    Created  : 31 Jul 2022 10:12:31pm
    Author   : Marcel Huibers
    Project  : 3D_knob_lnf
    Company  : Sound Development
    Copyright: Marcel Huibers (c) 2022 All Rights Reserved

  =====================================================================================================
*/

#include "custom_3d_knob_lnf.h"

custom_3d_knob_lnf::custom_3d_knob_lnf()
{
    // Sliders...
    setColour( juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack );
    setColour( juce::Slider::textBoxTextColourId, juce::Colours::white.darker() );
    setColour( juce::Slider::rotarySliderFillColourId, juce::Colours::darkslategrey.brighter() );
    setColour( juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkslategrey.darker() );
    setColour( juce::Slider::trackColourId, juce::Colours::white.darker() );
    setColour( juce::Slider::thumbColourId, juce::Colours::white.darker() );

    setColour( juce::TextEditor::ColourIds::outlineColourId, juce::Colours::transparentWhite );
    setColour( juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::transparentWhite );
}

void custom_3d_knob_lnf::drawRotarySlider( juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
                                           float rotaryEndAngle, juce::Slider& slider )
{
    // Knob constants...
    const bool     smallMode { ( width <= 60 ) };
    constexpr auto dialSizeRatio { 0.35f };
    constexpr auto dialBulbiness { 5 };
    const auto     baseSizeRatio { smallMode ? 0.8f : 0.7f };
    const auto     outerTickCircleRatio { smallMode ? 1.0f : 0.9f };
    const auto     innerTickCircleRatio { smallMode ? 0.9f : 0.8f };
    constexpr auto numTickSteps { 19 };
    constexpr auto tickHeightRatio { 0.7f };
    constexpr auto tickWidthRatio { 10.0f };

    // Knob center and angle...
    const auto  centerX = static_cast<float>( x ) + ( static_cast<float>( width ) / 2.0f );
    const auto  centerY = static_cast<float>( y ) + ( static_cast<float>( height ) / 2.0f );
    const float angle   = rotaryStartAngle + ( sliderPos * ( rotaryEndAngle - rotaryStartAngle ) );

    const auto baseColour  = findColour( juce::Slider::rotarySliderOutlineColourId );
    const auto knobColour  = findColour( juce::Slider::rotarySliderFillColourId );
    const auto trackColour = findColour( juce::Slider::trackColourId ).darker( slider.isEnabled() ? 0.0f : 0.4f );
    const auto thumbColour = findColour( juce::Slider::thumbColourId );


    // Draw base...
    const auto  baseDiameter = static_cast<float>( std::min( width, height ) ) * baseSizeRatio;
    const auto  baseRadius   = baseDiameter / 2.0f;
    const float rxBase       = centerX - baseRadius;
    const float ryBase       = centerY - baseRadius;
    juce::Path  dialBase;
    dialBase.addEllipse( rxBase, ryBase, baseDiameter, baseDiameter );
    g.setGradientFill( juce::ColourGradient( baseColour, rxBase, ryBase, baseColour.darker( 0.8f ), rxBase + baseRadius, ryBase + baseRadius, false ) );
    g.fillPath( dialBase );

    const auto outerDiameter = static_cast<float>( std::min( width, height ) ) * outerTickCircleRatio;
    if( !smallMode )
    {
        // Draw tick circle...
        const auto innerDiameter = static_cast<float>( std::min( width, height ) ) * innerTickCircleRatio;
        const auto outerRadius   = outerDiameter / 2.0f;
        const auto innerRadius   = innerDiameter / 2.0f;
        g.setColour( trackColour );
        juce::Path tickCircle;
        tickCircle.addCentredArc( centerX, centerY, outerRadius, outerRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true );
        tickCircle.addCentredArc( centerX, centerY, innerRadius, innerRadius, 0.0f, rotaryEndAngle, rotaryStartAngle, false );
        tickCircle.closeSubPath();
        g.strokePath( tickCircle, juce::PathStrokeType( 1.0f ) );

        // Draw tick marks...
        g.saveState();
        g.reduceClipRegion( tickCircle );
        g.setColour( trackColour );
        const auto angleStep = ( rotaryEndAngle - rotaryStartAngle ) / static_cast<float>( numTickSteps + 1 );
        auto       tickAngle = rotaryStartAngle + angleStep;
        while( tickAngle <= ( rotaryEndAngle - angleStep ) )
        {
            const auto endX = centerX + std::cos( tickAngle - juce::MathConstants<double>::halfPi ) * outerRadius;
            const auto endY = centerY + std::sin( tickAngle - juce::MathConstants<double>::halfPi ) * outerRadius;
            juce::Path tickLine;
            tickLine.addLineSegment( juce::Line<float>( centerX, centerY, endX, endY ), 1.0f );
            g.strokePath( tickLine, juce::PathStrokeType( 1.0f ) );

            tickAngle += angleStep;
        }
        g.restoreState();
    }

    // Draw inner dial...
    const auto  dialDiameter = static_cast<float>( std::min( width, height ) ) * dialSizeRatio;
    const auto  dialRadius   = dialDiameter / 2.0f;
    const float rxDial       = centerX - dialRadius;
    const float ryDial       = centerY - dialRadius;
    juce::Path  innerDial;
    innerDial.startNewSubPath( rxDial, ryDial );
    innerDial.quadraticTo( { rxDial + dialRadius, ryDial - dialBulbiness }, { rxDial + dialDiameter, ryDial } );
    innerDial.quadraticTo( { rxDial + dialDiameter + dialBulbiness, ryDial + dialRadius }, { rxDial + dialDiameter, ryDial + dialDiameter } );
    innerDial.quadraticTo( { rxDial + dialRadius, ryDial + dialDiameter + dialBulbiness }, { rxDial, ryDial + dialDiameter } );
    innerDial.quadraticTo( { rxDial - dialBulbiness, ryDial + dialRadius }, { rxDial, ryDial } );
    innerDial.closeSubPath();

    // Inner dial shadows...
    innerDial.applyTransform( juce::AffineTransform::rotation( angle, centerX, centerY ) );

    juce::Path innerDialRotated = innerDial;
    innerDialRotated.applyTransform( juce::AffineTransform::rotation( ( 45.0f / 360.0f ) * juce::MathConstants<float>::twoPi, centerX, centerY ) );

    const auto shadow = juce::DropShadow( juce::Colours::black, 20, { static_cast<int>( baseRadius ) / 4, static_cast<int>( baseRadius ) / 4 } );
    shadow.drawForPath( g, innerDial );
    shadow.drawForPath( g, innerDialRotated );

    // Draw dial...
    if( slider.isEnabled() )
    {
        const float tickWidth = juce::jlimit( 2.0f, 40.0f, baseDiameter / tickWidthRatio );
        juce::Path  dialTick;
        dialTick.addTriangle( centerX,
                              centerY - baseRadius,
                              centerX + ( tickWidth / 2.0f ),
                              centerY - ( baseRadius * tickHeightRatio ),
                              centerX - ( tickWidth / 2.0f ),
                              centerY - ( baseRadius * tickHeightRatio ) );
        g.setColour( thumbColour );
        g.fillPath( dialTick, juce::AffineTransform::rotation( angle, centerX, centerY ) );  // NOLINT
    }

    g.setColour( knobColour );
    g.fillPath( innerDial );
    g.fillPath( innerDialRotated );

    // Draw values...
    if( !smallMode )
    {
        g.setFont( juce::Font( juce::jlimit( 10.0f, 20.0f, outerDiameter / 8.0f ) ) );
        const auto textSize = juce::jlimit( 20, 80, width - juce::roundToInt( centerX + baseRadius ) );
        g.setColour( trackColour );
        const auto textX1 = juce::roundToInt( centerX - baseRadius );
        g.drawFittedText( slider.getTextFromValue( slider.getMinimum() ), x, static_cast<int>( centerY + baseRadius ), textX1 - x, textSize, juce::Justification::topRight, 1 );
        g.drawFittedText( slider.getTextFromValue( slider.getMaximum() ), centerX + baseRadius, static_cast<int>( centerY + baseRadius ), textSize, textSize, juce::Justification::topLeft, 1 );
    }
}
//==============================================================================
