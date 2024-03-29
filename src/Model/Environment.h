/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Nutrient.h"
#include "Autotrophs.h"
#include "Heterotrophs.h"

class Environment {
public:
    Environment( );
    ~Environment( );

    void Update( );
    bool RecordData( );

    Nutrient& GetNutrient( );
    Autotrophs& GetAutotrophs( );
    Heterotrophs& GetHeterotrophs( );

private:
    Nutrient mNutrient;
    Autotrophs mAutotrophs;
    Heterotrophs mHeterotrophs;
};

#endif

