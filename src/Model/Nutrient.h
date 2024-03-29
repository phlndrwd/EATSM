/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef NUTRIENT
#define	NUTRIENT

#include "Types.h"

class Nutrient {
public:
    Nutrient( );
    ~Nutrient( );
    
    void RecordData( );
    
    double GetVolume( );
    void SetVolume( const double );
    
    void AddToVolume( const double );
    void SubtractFromVolume( const double );
    
private:
    double mVolume;
    double mToFlux;

};

#endif

