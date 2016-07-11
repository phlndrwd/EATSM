#ifndef AUTOTROPH
#define	AUTOTROPH

#include "Types.h"

class Autotroph {
public:
    Autotroph( Types::NutrientPointer );
    ~Autotroph( );
    
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

