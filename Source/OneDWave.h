/*
  ==============================================================================

    OneDWave.h
    Created: 24 Mar 2022 8:38:24am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OneDWave  : public juce::Component
{
public:
    OneDWave (double k); // initialise the model with the time step
    ~OneDWave() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // Function containing the update equation.
    void calculateScheme();
    
    // Update the states by performing a pointer switch.
    void updateStates();
    
    // Function to visualise the state of the system.
    Path visualiseState (Graphics& g);
    
    // Function to excite the system.
    void excite();
    
    // Get the output at a specified ratio of the length of the system
    float getOutput (float outRatio) { return u[1][(int)floor(outRatio * N)];};
    
private:
    
    // Variables
    
    double k; // time step
    double c; // Wave speed (in m/s)
    double h; // Grid spacing (in m)
    double lambdaSq; // Courant number to be used in the update equation
    
    int N; // number of intervals (number of grid points is N+1)

    // A vector of 3 vectors saving the states of the system at n+1, n and n-1
    std::vector<std::vector<double>> uStates;
    
    // Pointers to the 3 vectors in uStates. The first index indicates the time index (0 -> n+1, 1 -> n, 2 -> n-1) and the second index indicates the spatial index.
    /*
     Examples:
         u[0][3] -> u_{3}^{n+1}
         u[1][7] -> u_{7}^{n}
         u[2][50] -> u_{50}^{n-1}
     */
    std::vector<double*> u;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneDWave)
};
