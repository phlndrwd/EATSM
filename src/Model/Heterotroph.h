/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef HETEROTROPH
#define HETEROTROPH

#include "Types.h"
#include "HeterotrophProcessor.h"
#include "HeritableTraits.h"
#include "RandomSimple.h"

class Heterotroph {
public:
    Heterotroph( const HeritableTraits&, const double, const unsigned );
    Heterotroph( const HeritableTraits&, const double, const double, const double, const double, const unsigned );
    Heterotroph( const HeritableTraits&, const double, const double, const unsigned );
    Heterotroph( const HeritableTraits&, const double, const double, const double, const double, const double, const double, const unsigned, const unsigned, const bool, const bool );
    Heterotroph( const Heterotroph& );
    Heterotroph( const Heterotroph&& ) noexcept;
    ~Heterotroph( );
    Heterotroph& operator = ( const Heterotroph& );
    bool operator == ( const Heterotroph& );

    Types::HeterotrophPointer Reproduce( RandomSimple&, HeterotrophProcessor& );

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
    
    const double mAssimilationEfficiency;

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
