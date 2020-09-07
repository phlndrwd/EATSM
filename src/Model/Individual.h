#ifndef INDIVIDUAL
#define INDIVIDUAL

#include "Types.h"
#include "HeritableTraits.h"

class Individual {
public:
    Individual( const HeritableTraits&, const double, const unsigned );
    Individual( const HeritableTraits&, const double, const double, const double, const double, const unsigned );
    Individual( const HeritableTraits&, const double, const double, const unsigned );
    Individual( const HeritableTraits&, const double, const double, const double, const double, const double, const double, const unsigned, const unsigned, const bool, const bool );
    Individual( const Individual& );
    Individual( const Individual&& ) noexcept;
    ~Individual( );
    Individual& operator = ( const Individual& individual );
    bool operator == ( const Individual& );

    Types::IndividualPointer Reproduce( );

    double ConsumePreyVolume( const double );
    double Metabolise( const double );

    HeritableTraits& GetHeritableTraits( );
    double GetTrophicLevel( ) const;
    unsigned GetSizeClassIndex( ) const;
    unsigned GetAge( ) const;

    bool HasFed( ) const;
    bool IsDead( ) const;

    double GetVolumeActual( ) const;
    double GetVolumeHeritable( ) const;
    double GetVolumeMinimum( ) const;
    double GetVolumeReproduction( ) const;

    double GetStarvationMultiplier( ) const;

    void SetTrophicLevel( const double );
    void SetSizeClassIndex( const unsigned );

    void SetAge( const unsigned );
    void SetHasFed( const bool );
    void Kill( );

private:
    HeritableTraits mHeritableTraits;

    double mVolumeHeritable;
    double mVolumeMinimum;
    double mVolumeReproduction;

    double mVolumeActual;
    double mTrophicLevel;

    double mStarvationMultiplier;

    unsigned mSizeClassIndex;
    unsigned mAge;

    bool mHasFed;
    bool mIsDead;
};

#endif
