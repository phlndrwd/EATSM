/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TIMESTEP
#define	TIMESTEP

class TimeStep {
public:
    TimeStep( );
    ~TimeStep( );

    unsigned GetTimeStep( ) const;
    
    bool DoRecordData( ) const;
    void IncrementTimeStep( );

private:
    const unsigned mSamplingRate;
    
    unsigned mTimeStep;
};

#endif

