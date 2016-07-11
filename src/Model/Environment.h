#ifndef ENVIRONMENT
#define	ENVIRONMENT

#include "Types.h"

class Environment {
public:
    Environment( );
    ~Environment( );

    void Update( );
    bool RecordData( );

private:
    Types::NutrientPointer mNutrient;
    Types::AutotrophPointer mPhytoplankton;
    Types::HeterotrophsPointer mHeterotrophs;
};

#endif

