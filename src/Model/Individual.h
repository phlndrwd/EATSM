#ifndef INDIVIDUAL
#define	INDIVIDUAL

#include "Constants.h"
#include "Types.h"

class Individual {
public:
    Individual( const double, const unsigned int );
    Individual( const Types::GenomePointer, const double, const double, const double, const double );
    Individual( const Types::GenomePointer, const double, const double, const unsigned int, const unsigned int );
    ~Individual( );

    Types::IndividualPointer Reproduce( );

    double ConsumePreyVolume( const double );
    double Metabolise( const double );

    Types::GenomePointer GetGenome( ) const;
    double GetTrophicLevel( ) const;
    unsigned int GetSizeClassIndex( ) const;

    unsigned int GetAge( ) const;

    bool IsDead( ) const;

    double GetVolumeActual( ) const;
    double GetVolumeHeritable( ) const;
    double GetVolumeMinimum( ) const;
    double GetVolumeReproduction( ) const;

    void SetTrophicLevel( const double );
    void SetSizeClassIndex( const unsigned int );

    void SetAge( const unsigned int );
    void Kill( );
    void SetTagNumber( const int );

private:
    Types::GenomePointer mGenome;

    double mVolumeActual;
    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;
    
    unsigned int mSizeClassIndex;

    double mTrophicLevel;

    unsigned int mAge;
    bool mIsDead;
};

#endif
