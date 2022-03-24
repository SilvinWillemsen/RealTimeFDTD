#pragma once

#include <JuceHeader.h>

///__________________________

#include "OneDWave.h"

///__________________________

class MainComponent  : public juce::AudioAppComponent,
///__________________________
                       public Timer // timer to update the graphics
///__________________________

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    ///__________________________
    
    // The timer callback function that updates the graphics.
    void timerCallback() override;
    
    // Limiter to keep the output signal between -1 and 1. Arguments are: input value, minimum value and maximum value
    float limit (float val, float min, float max);

    ///__________________________

private:
    ///__________________________
    
    double fs; // Sample rate which we can retrieve from the prepareToPlay function
    
    std::unique_ptr<OneDWave> oneDWave; // using a smart pointer for 1D wave class

    ///__________________________
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
