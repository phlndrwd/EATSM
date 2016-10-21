#ifndef INDIVIDUAL
#define	INDIVIDUAL

#include "Constants.h"
#include "Types.h"

class Individual {
public:
    Individual( const double, const unsigned );
    Individual( const Types::GenomePointer, const double, const double, const double, const double );
    Individual( const double, const double, const unsigned );
    ~Individual( );

    Types::IndividualPointer Reproduce( );

    double ConsumePreyVolume( const double );
    double Metabolise( const double );
    
    Types::GenomePointer GetGenome( ) const;
    double GetTrophicLevel( ) const;
    unsigned GetSizeClassIndex( ) const;

    unsigned GetAge( ) const;

    bool IsDead( ) const;

    double GetVolumeActual( ) const;
    double GetVolumeHeritable( ) const;
    double GetVolumeMinimum( ) const;
    double GetVolumeReproduction( ) const;
    
    void SetTrophicLevel( const double );
    void SetSizeClassIndex( const unsigned );

    void SetAge( const unsigned );
    void Kill( );

private:
    Types::GenomePointer mGenome;

    double mVolumeActual;
    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;
    
    unsigned mSizeClassIndex;

    double mTrophicLevel;

    unsigned mAge;
    bool mIsDead;
};

#endif
