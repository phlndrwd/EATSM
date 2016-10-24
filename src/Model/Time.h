#ifndef TIME
#define	TIME

#include "Types.h"

class Time {
public:
    ~Time( );
    static Types::TimePointer Get( );
    
    unsigned GetTimeStep( ) const;
    double* GetTimeStepPointer( );
    bool DoRecordData( ) const;
    void IncrementTimeStep( );
    
private:
    Time( );
    
    static Types::TimePointer mThis;
    unsigned mTimeStep;
    double mFloatTimeStep;
};

#endif

