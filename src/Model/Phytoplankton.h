#ifndef PHYTOPLANKTON
#define	PHYTOPLANKTON

#include "Types.h"

class Phytoplankton {
public:
    Phytoplankton( Types::NutrientPointer );
    ~Phytoplankton( );
    
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

