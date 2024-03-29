/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "TimeStep.h"
#include "Parameters.h"

TimeStep::TimeStep( ):
mSamplingRate( Parameters::Get( )->GetSamplingRate( ) ) {
    mTimeStep = 0;
}

TimeStep::~TimeStep( ) {

}

unsigned TimeStep::GetTimeStep( ) const {
    return mTimeStep;
}

bool TimeStep::DoRecordData( ) const {
    return ( mTimeStep % mSamplingRate == 0 );
}

void TimeStep::IncrementTimeStep( ) {
    ++mTimeStep;
}