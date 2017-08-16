#ifndef AUTOTROPHS
#define	AUTOTROPHS

#include "Types.h"

class Autotrophs {
public:
    Autotrophs( );
    Autotrophs( Types::NutrientPointer );
    ~Autotrophs( );
    
    void RecordData( );
    
    void Update( );

    double GetVolume( );
    void SetVolume( const double );

    void AddToVolume( const double );
    void SubtractFromVolume( const double );
    
private:
    Types::NutrientPointer mNutrient;
    
    double mVolume;
    double mToFlux;

};

#endif

