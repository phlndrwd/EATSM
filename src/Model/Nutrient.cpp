/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Nutrient.h"
#include "InitialState.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Constants.h"

#include <iostream>

Nutrient::Nutrient( ) {
    if( InitialState::Get( )->IsInitialised( ) == true )
        mVolume = InitialState::Get( )->GetNutrientVolume( );
    else
        mVolume = 0;
    
    std::cout << "Nutrient pool created." << std::endl;
}

Nutrient::~Nutrient( ) {

}

void Nutrient::RecordData( ) {
    DataRecorder::Get( )->AddDataTo( "NutrientVolume", mVolume );
    DataRecorder::Get( )->AddDataTo( "ToNutrientFlux", mToFlux );
    mToFlux = 0;
}

double Nutrient::GetVolume( ) {
    return mVolume;
}

void Nutrient::SetVolume( const double volume ) {
    mVolume = volume;
}

void Nutrient::AddToVolume( const double volume ) {
    mVolume += volume;
    mToFlux += volume;
}

void Nutrient::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}
