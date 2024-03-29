/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TIMER
#define	TIMER

#include <string>
#include <chrono>

class Timer {
public:
    Timer( bool start = true );
    ~Timer( );
    
    void Start( );
    double Split( );
    double Elapsed( );
    double Stop( );
    
    std::string RemainingString( );
    
private:
    const unsigned mRunTimeInSeconds;
    
    std::chrono::high_resolution_clock::time_point mStartTime;
    std::chrono::high_resolution_clock::time_point mSplitTime;
    std::chrono::high_resolution_clock::time_point mElapsedTime;
    std::chrono::high_resolution_clock::time_point mStopTime;
};

#endif

