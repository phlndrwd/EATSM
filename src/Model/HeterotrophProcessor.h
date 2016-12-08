#ifndef HETEROTROPHPROCESSOR
#define	HETEROTROPHPROCESSOR

#include "Types.h"

class HeterotrophProcessor {
public:
    HeterotrophProcessor( );
    ~HeterotrophProcessor( );

    double CalculatePreferenceForPrey( const double, const double ) const;

    double CalculateFeedingProbability( const double );

    double CalculateMetabolicDeduction( const Types::IndividualPointer ) const;
    double CalculateStarvationProbability( const Types::IndividualPointer ) const;

    unsigned FindAndSetSizeClassIndex( const Types::IndividualPointer ) const;
    unsigned FindSizeClassIndexFromVolume( const double ) const;
    
    bool ShouldIndividualMoveSizeClass( const Types::IndividualPointer ) const;
    
    const double GeneValueToVolume( double ) const;
    const double VolumeToGeneValue( double ) const;
    
    int RoundWithProbability( const double& ) const;

private:
    double CalculateFeedingProbabilityType1( const double ) const;
    double CalculateFeedingProbabilityType2( const double ) const;

    double CalculateLinearStarvation( const double, const double, const double ) const;
    double CalculateBetaExponentialStarvation( const double, const double, const double ) const;
};

#endif

