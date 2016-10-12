#ifndef ENVIRONMENT
#define	ENVIRONMENT

#include "Types.h"

class Environment {
public:
    Environment( );
    ~Environment( );

    void Update( );
    bool RecordData( );
    
    Types::NutrientPointer GetNutrient( ) const;
    Types::AutotrophsPointer GetAutotrophs( ) const;
    Types::HeterotrophsPointer GetHeterotrophs( ) const;

private:
    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mAutotrophs;
    Types::HeterotrophsPointer mHeterotrophs;
};

#endif

