#ifndef INDIVIDUAL
#define	INDIVIDUAL

#include "Constants.h"
#include "Types.h"

class Individual {
public:
    Individual( Types::HeterotrophProcessorPointer, const double, const unsigned );
    Individual( const Types::HeritableTraitsPointer, const double, const double, const double, const double );
    Individual( Types::HeterotrophProcessorPointer, const double, const double, const unsigned );
    ~Individual( );

    Types::IndividualPointer Reproduce( Types::HeterotrophProcessorPointer );

    double ConsumePreyVolume( const double, bool isHerbivory = true );
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
    unsigned mSizeClassIndex;
    double mTrophicLevel;
    unsigned mAge;

    bool mIsDead;
};

#endif
