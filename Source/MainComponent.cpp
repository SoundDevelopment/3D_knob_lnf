#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(m_demoKnob);
    
    juce::LookAndFeel::setDefaultLookAndFeel(&m_3dKnobLook);
    m_demoKnob.setTextValueSuffix( "%" );
    m_demoKnob.setRange( { 0.0, 100.0 }, 0.0 );
    m_demoKnob.setNumDecimalPlacesToDisplay( 1 );
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    juce::LookAndFeel::setDefaultLookAndFeel( nullptr );
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    m_demoKnob.setBounds( getLocalBounds() );
}
