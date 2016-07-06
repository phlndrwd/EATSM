#ifndef ENVIRONMENT
#define	ENVIRONMENT

#include "Types.h"

class Environment {
public:
    Environment( );
    ~Environment( );

    void Update( );
    bool RecordData( );
    void OutputHeterotrophSummaryData( ) const;
    
    Types::NutrientPointer GetNutrient( );
    Types::PhytoplanktonPointer GetPhytoplankton( );
    Types::HeterotrophsPointer GetHeterotrophs( );

private:

    Types::NutrientPointer mNutrient;
    Types::PhytoplanktonPointer mPhytoplankton;
    Types::HeterotrophsPointer mHeterotrophs;
};

#endif

