#ifndef INDIVIDUAL
#define	INDIVIDUAL

#include "Constants.h"
#include "Types.h"

class Individual {
public:
    Individual( const double, const double, const unsigned );
    Individual( const Types::HeritableTraitsPointer, const double, const double, const double, const double );
    Individual( const double, const double, const double, const unsigned );
    ~Individual( );

    Types::IndividualPointer Reproduce( );

    double ConsumePreyVolume( const double );
    double Metabolise( const double );

    Types::HeritableTraitsPointer GetHeritableTraits( ) const;
    double GetTrophicLevel( ) const;
    unsigned GetSizeClassIndex( ) const;   
    unsigned GetAge( ) const;

    bool IsDead( ) const;

    double GetVolumeActual( ) const;
    double GetVolumeHeritable( ) const;
    double GetVolumeMinimum( ) const;
    double GetVolumeReproduction( ) const;
    
    double GetStarvationMultiplier( ) const;

    void SetTrophicLevel( const double );
    void SetSizeClassIndex( const unsigned );

    void SetAge( const unsigned );
    void Kill( );

private:
    Types::HeritableTraitsPointer mHeritableTraits;

    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;

    double mVolumeActual;
    double mTrophicLevel;
    
    double mStarvationMultiplier;
    
    unsigned mSizeClassIndex;
    unsigned mAge;

    bool mIsDead;
};

#endif
