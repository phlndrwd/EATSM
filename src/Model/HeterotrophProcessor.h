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

    static double TraitValueToVolume( double );
    double VolumeToTraitValue( double ) const;

    int RoundWithProbability( const double& ) const;

private:
    double CalculateFeedingProbabilityLinear( const unsigned, const double );
    double CalculateFeedingProbabilityNonLinear( const unsigned, const double );

    double CalculateLinearStarvation( const double&, const double&, const double&, const double& ) const;
    double CalculateBetaExponentialStarvation( const double&, const double&, const double&, const double& ) const;

    typedef double( HeterotrophProcessor::*function )( const unsigned, const double );
    double ( HeterotrophProcessor::*fStarvationProbability )( const unsigned, const double );
};

#endif

