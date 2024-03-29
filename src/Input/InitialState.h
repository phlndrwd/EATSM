/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef INITIALSTATE
#define	INITIALSTATE

#include "Types.h"

class InitialState {
public:
    ~InitialState( );
    static Types::InitialStatePointer Get( );

    bool Initialise( const Types::StringMatrix& );
    bool IsInitialised( );

    double& GetNutrientVolume( );
    double& GetAutotrophVolume( );
    Types::HeterotrophMatrix& GetHeterotrophs( );
    unsigned& GetInitialPopulationSize( );

private:
    InitialState( );
    static Types::InitialStatePointer mThis;

    Types::HeterotrophMatrix mHeterotrophs;
    
    bool mIsInitialised;
    double mNutrientVolume;
    double mAutotrophVolume;
    unsigned mInitialPopulationSize;
};

#endif

