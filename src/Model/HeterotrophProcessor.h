#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include "Types.h"

class HeterotrophProcessor {
public:
    HeterotrophProcessor( );
    ~HeterotrophProcessor( );

    double CalculatePreferenceForPrey( const double, const double ) const;

    double CalculateFeedingProbability( const double );

    double CalculateMetabolicDeduction( const Types::IndividualPointer ) const;
    double CalculateStarvationProbability( const Types::IndividualPointer ) const;

    bool UpdateSizeClassIndex( const Types::IndividualPointer ) const;
    unsigned FindSizeClassIndexFromVolume( const double ) const;
    unsigned FindIndividualSizeClassIndex( const Types::IndividualPointer, unsigned ) const;
    unsigned DirectionIndividualShouldMoveSizeClasses( const Types::IndividualPointer ) const;

    static double TraitValueToVolume( double );
    double VolumeToTraitValue( double ) const;

    int RoundWithProbability( const double& ) const;

private:
    double CalculateFeedingProbabilityType1( const double ) const;
    double CalculateFeedingProbabilityType2( const double ) const;

    double CalculateLinearStarvation( const double&, const double&, const double&, const double& ) const;
    double CalculateBetaExponentialStarvation( const double&, const double&, const double&, const double& ) const;
};

#endif

