#include "HeterotrophProcessor.h"

#include "Parameters.h"
#include "Individual.h"
#include "Maths.h"

HeterotrophProcessor::HeterotrophProcessor( ) {

}

HeterotrophProcessor::~HeterotrophProcessor( ) {

}

double HeterotrophProcessor::CalculatePreferenceForPrey( const double grazerVolume, const double preyVolume ) const {
    return CalculateNormalPreferenceForPrey( grazerVolume, preyVolume );
}

double HeterotrophProcessor::CalculateNormalPreferenceForPrey( const double grazerVolume, const double preyVolume ) const {
    return Parameters::Get( )->GetPreferenceFunctionHeight( ) * Maths::Get( )->Exp( -Maths::Get( )->Pow( ( Maths::Get( )->Log( ( Parameters::Get( )->GetPreferredPreyVolumeRatio( ) * preyVolume ) / grazerVolume ) ), 2 ) / ( 2 * Maths::Get( )->Pow( Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) ) );
}

double HeterotrophProcessor::CalculateParabolicPreferenceForPrey( const double grazerVolume, const double preyVolume ) const {
    return Maths::Get( )->Max( 0, Parameters::Get( )->GetPreferenceFunctionHeight( ) * 1 - Maths::Get( )->Pow( ( Maths::Get( )->Log10( ( grazerVolume / Parameters::Get( )->GetPreferredPreyVolumeRatio( ) ) / preyVolume ) ) / Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) );
}

double HeterotrophProcessor::CalculateFeedingProbability( const double effectivePreyVolume ) {
    return CalculateFeedingProbabilityType2( effectivePreyVolume );
}

double HeterotrophProcessor::CalculateMetabolicDeduction( const Types::IndividualPointer individual ) const {
    return Parameters::Get( )->GetFractionalMetabolicExpense( ) * Maths::Get( )->Pow( individual->GetVolumeActual( ), Parameters::Get( )->GetMetabolicIndex( ) );
}

double HeterotrophProcessor::CalculateStarvationProbability( const Types::IndividualPointer individual ) const {
    return CalculateLinearStarvation( individual->GetVolumeActual( ), individual->GetVolumeHeritable( ), individual->GetVolumeMinimum( ) );
}

unsigned HeterotrophProcessor::FindAndSetSizeClassIndex( const Types::IndividualPointer individual ) const {
    unsigned sizeClassIndex = FindSizeClassIndexFromVolume( individual->GetVolumeActual( ) );
    individual->SetSizeClassIndex( sizeClassIndex );

    return sizeClassIndex;
}

unsigned HeterotrophProcessor::FindSizeClassIndexFromVolume( const double volume ) const {
    unsigned sizeClassIndex = 0;

    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );

    if( volume >= Parameters::Get( )->GetSizeClassBoundary( numberOfSizeClasses - 1 ) ) {
        sizeClassIndex = numberOfSizeClasses - 1;
    } else {
        for( unsigned potentialIndex = 1; potentialIndex <= numberOfSizeClasses; ++potentialIndex ) {
            if( volume < Parameters::Get( )->GetSizeClassBoundary( potentialIndex ) ) {
                sizeClassIndex = potentialIndex - 1;
                break;
            }
        }
    }
    return sizeClassIndex;
}

bool HeterotrophProcessor::ShouldIndividualMoveSizeClass( const Types::IndividualPointer individual ) const {
    bool individualShouldMoveSizeClass = false;

    unsigned sizeClassIndex = individual->GetSizeClassIndex( );
    double volumeActual = individual->GetVolumeActual( );

    if( sizeClassIndex == 0 ) {

        if( volumeActual >= Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex + 1 ) ) {
            individualShouldMoveSizeClass = true;
        }
    } else if( sizeClassIndex == Parameters::Get( )->GetNumberOfSizeClasses( ) - 1 ) {

        if( volumeActual < Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex ) ) {
            individualShouldMoveSizeClass = true;
        }
    } else {
        if( volumeActual >= Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex + 1 ) || volumeActual < Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex ) ) {
            individualShouldMoveSizeClass = true;
        }
    }
    return individualShouldMoveSizeClass;
}

double HeterotrophProcessor::CalculateFeedingProbabilityType1( const double effectivePreyVolume ) const {
    return ( effectivePreyVolume / Parameters::Get( )->GetTotalVolume( ) );
}

double HeterotrophProcessor::CalculateFeedingProbabilityType2( const double effectivePreyVolume ) const {
    return ( effectivePreyVolume / ( Parameters::Get( )->GetHalfSaturationConstant( ) + effectivePreyVolume ) );
}

double HeterotrophProcessor::CalculateLinearStarvation( const double volumeActual, const double volumeHeritable, const double volumeMinimum ) const {

    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum ) {
        starvationProbability = 1;
    } else if( volumeActual >= volumeHeritable ) {
        starvationProbability = 0;
    } else {
        starvationProbability = 1 + ( ( volumeMinimum - volumeActual ) / ( volumeHeritable - volumeMinimum ) );
    }

    return starvationProbability;
}

double HeterotrophProcessor::CalculateBetaSigmoidStarvation( const double volumeActual, const double volumeHeritable, const double volumeMinimum ) const {

    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum ) {
        starvationProbability = 1;
    } else if( volumeActual >= volumeHeritable ) {
        starvationProbability = 0;
    } else {
        starvationProbability = 1 - ( 1 + ( ( volumeActual - volumeMinimum ) - ( volumeActual - volumeMinimum ) ) / ( ( volumeActual - volumeMinimum ) - ( ( volumeActual - volumeMinimum ) / 2 ) ) ) * Maths::Get( )->Pow( ( ( volumeActual - volumeMinimum ) / ( volumeActual - volumeMinimum ) ), ( ( volumeActual - volumeMinimum ) / ( ( volumeActual - volumeMinimum ) - ( ( volumeActual - volumeMinimum ) / 2 ) ) ) );
    }

    return starvationProbability;
}

double HeterotrophProcessor::CalculateBetaExponentialStarvation( const double volumeActual, const double volumeHeritable, const double volumeMinimum ) const {

    double starvationProbability = -1;

    if( volumeActual <= volumeMinimum ) {
        starvationProbability = 1;
    } else if( volumeActual >= volumeHeritable ) {
        starvationProbability = 0;
    } else {
        starvationProbability = 1 - ( 1 + ( ( volumeHeritable - volumeMinimum ) - ( volumeActual - volumeMinimum ) ) / ( ( volumeHeritable - volumeMinimum ) ) ) * ( ( volumeActual - volumeMinimum ) / ( volumeHeritable - volumeMinimum ) );
    }

    return starvationProbability;
}
