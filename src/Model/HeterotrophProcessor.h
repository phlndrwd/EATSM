#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include "Types.h"

class HeterotrophProcessor {
public:
    HeterotrophProcessor( );
    ~HeterotrophProcessor( );

    double CalculatePreferenceForPrey( const double, const double ) const;
    double CalculateFeedingProbability( const unsigned, const double );

    double CalculateMetabolicDeduction( const Types::IndividualPointer ) const;
    double CalculateStarvationProbability( const Types::IndividualPointer ) const;

    bool UpdateSizeClassIndex( Types::IndividualPointer ) const;
    unsigned FindSizeClassIndexFromVolume( const double ) const;
    unsigned FindIndividualSizeClassIndex( const Types::IndividualPointer, unsigned ) const;
    unsigned DirectionIndividualShouldMoveSizeClasses( const Types::IndividualPointer ) const;
    
    void UpdateHerbivoreTrophicIndex( const Types::IndividualPointer ) const;
    void UpdateCarnivoreTrophicIndex( const Types::IndividualPointer, Types::IndividualPointer ) const;

    double TraitValueToVolume( double );
    double VolumeToTraitValue( double ) const;

    int RoundWithProbability( const double& ) const;

private:
    double CalculateFeedingProbabilityLinear( const unsigned, const double );
    double CalculateFeedingProbabilityNonLinear( const unsigned, const double );

    double CalculateLinearStarvation( const double&, const double&, const double&, const double& ) const;
    double CalculateBetaExponentialStarvation( const double&, const double&, const double&, const double& ) const;

    typedef double( HeterotrophProcessor::*function )( const unsigned, const double );
    double ( HeterotrophProcessor::*fStarvationProbability )( const unsigned, const double );
    
    const Types::DoubleVector mSizeClassBoundaries;
    const Types::DoubleVector mLinearFeedingDenominators;
    const Types::DoubleVector mHalfSaturationConstants;
    
    const double mPreferredPreyVolumeRatio;
    const double mPreferenceFunctionWidth;
    const double mFractionalMetabolicExpense;
    const double mMetabolicIndex;
    const double mNumberOfSizeClasses;
    
    const double mLargestVolumeExponent;
    const double mSmallestVolumeExponent;
    
    const double mPreferenceDenominator;
};

#endif

