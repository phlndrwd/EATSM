#ifndef TIMESTEP
#define	TIMESTEP

#include "Types.h"

class TimeStep {
public:
    ~TimeStep( );
    static Types::TimePointer Get( );

    unsigned GetTimeStep( ) const;
    
    bool DoRecordData( ) const;
    void IncrementTimeStep( );

private:
    TimeStep( );

    static Types::TimePointer mThis;
    unsigned mTimeStep;
};

#endif

