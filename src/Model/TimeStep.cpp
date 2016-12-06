#include "TimeStep.h"

#include "Parameters.h"

Types::TimePointer TimeStep::mThis = NULL;

TimeStep::TimeStep( ) {
    mTimeStep = 0;
}

TimeStep::~TimeStep( ) {
    if( mThis != NULL ) delete mThis;
}

Types::TimePointer TimeStep::Get( ) {
    if( mThis == NULL ) mThis = new TimeStep( );

    return mThis;
}

unsigned TimeStep::GetTimeStep( ) const {
    return mTimeStep;
}

bool TimeStep::DoRecordData( ) const {
    return ( mTimeStep % Parameters::Get( )->GetSamplingRate( ) == 0 );
}

void TimeStep::IncrementTimeStep( ) {
    ++mTimeStep;
}