#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    m_demoKnob.setLookAndFeel (&m_3dKnobLook);
    m_demoKnob.setTextValueSuffix ("%");
    m_demoKnob.setRange ({ 0.0, 100.0 }, 0.0);
    m_demoKnob.setNumDecimalPlacesToDisplay (1);
    addAndMakeVisible(m_demoKnob);

    setSize (200, 200);
}

MainComponent::~MainComponent()
{
    m_demoKnob.setLookAndFeel (nullptr);
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
    m_demoKnob.setBounds( getLocalBounds().reduced(5) );
}
