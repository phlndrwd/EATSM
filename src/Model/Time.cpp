#include "Time.h"

#include "Parameters.h"

Types::TimePointer Time::mThis = NULL;

Time::Time( ) {
    mTimeStep = 0;
    mFloatTimeStep = mTimeStep;
}

Time::~Time( ) {
    if( mThis != NULL ) delete mThis;
}

Types::TimePointer Time::Get( ) {
    if( mThis == NULL ) mThis = new Time( );

    return mThis;
}

unsigned Time::GetTimeStep( ) const {
    return mTimeStep;
}

double* Time::GetTimeStepPointer( ) {
    return &mFloatTimeStep;
}

bool Time::DoRecordData( ) const {
    return ( mTimeStep % Parameters::Get( )->GetSamplingRate( ) == 0 );
}

void Time::IncrementTimeStep( ) {
    ++mTimeStep;
    mFloatTimeStep = mTimeStep;
}