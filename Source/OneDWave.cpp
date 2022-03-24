/*
  ==============================================================================

    OneDWave.cpp
    Created: 24 Mar 2022 8:38:24am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OneDWave.h"

//==============================================================================
OneDWave::OneDWave (double kIn) : k (kIn) // <- This is an initialiser list. It initialises the member variable 'k' (in the "private" section in OneDWave.h), using the argument of the constructor 'kIn'.
{
    
    //******

    c = 300;
    h = c * k;
    L = 1;
    N = floor(L / h);
    h = L / N;
    lambdaSq = c * c * k * k / (h * h);
    
    //******
    
//    // ONLY FOR TESTING PURPOSES, you'll have to change this.
//    N = 100;
    
    
    // Initialise vectors containing the state of the system
    uStates = std::vector<std::vector<double>> (3,
                                        std::vector<double>(N+1, 0));
    
    // Initialise vector of pointers to the states
    u.resize (3, nullptr);
    
    // Make set memory addresses to first index of the state vectors.
    for (int i = 0; i < 3; ++i)
        u[i] = &uStates[i][0];
    
    // Excite at the start halfway along the system.
    excite (0.5);

}

OneDWave::~OneDWave()
{
}

void OneDWave::paint (juce::Graphics& g)
{
    // Set the colour of the path
    g.setColour(Colours::cyan);
    
    // Generate the path that visualises the state of the system.
    Path visualStatePath = visualiseState (g);
    
    // Draw the path using a stroke (thickness) of 2 pixels.
    g.strokePath (visualStatePath, PathStrokeType(2.0f));
}

void OneDWave::resized()
{
}

void OneDWave::calculateScheme()
{
    // Here is where you'll have to implement your update equation in a for loop (ranging from l = 1 to l < N).
    
    //******
    
    for (int l = 1; l < N; ++l)
        u[0][l] = 2 * u[1][l] - u[2][l] + lambdaSq * (u[1][l+1] - 2 * u[1][l] + u[1][l-1]);
    
    //******
}

void OneDWave::updateStates()
{
    // Here is where you'll have to implement the pointer switch.
    
    //******

    double* uTmp = u[2];
    u[2] = u[1];
    u[1] = u[0];
    u[0] = uTmp;
    
    //******
}

Path OneDWave::visualiseState (Graphics& g)
{
    double visualScaling = 200; // we have to scale up the state of the system from 'transverse displacement' to 'pixels'
    
    // String-boundaries are in the vertical middle of the component
    double stringBoundaries = getHeight() / 2.0;
    
    // Initialise path
    Path stringPath;
    
    // Start path
    stringPath.startNewSubPath (0, -u[1][0] * visualScaling + stringBoundaries);
    
    // Visual spacing between two grid points
    double spacing = getWidth() / static_cast<double>(N);
    double x = spacing;
    
    for (int l = 1; l <= N; l++)
    {
        // Needs to be -u, because a positive u would visually go down
        float newY = -u[1][l] * visualScaling + stringBoundaries;
        
        // if we get NAN values, make sure that we don't get an exception
        if (isnan(newY))
            newY = 0;
        
        stringPath.lineTo (x, newY);
        x += spacing;
    }
    
    return stringPath;
}

void OneDWave::excite (double excitationLoc)
{
    
    // width (in grid points) of the excitation
    double width = 10;
    
    // make sure we're not going out of bounds at the left boundary
    int start = std::max (floor((N+1) * excitationLoc) - floor(width * 0.5), 1.0);

    for (int l = 0; l < width; ++l)
    {
        // make sure we're not going out of bounds at the right boundary (this does 'cut off' the raised cosine)
        if (l+start > N - 1)
            break;
        
        u[1][l+start] += 0.5 * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
        u[2][l+start] += 0.5 * (1 - cos(2.0 * MathConstants<double>::pi * l / (width-1.0)));
    }
}
