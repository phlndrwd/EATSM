#include "HeterotrophProcessor.h"

#include "Parameters.h"
#include "Individual.h"
#include "Maths.h"

HeterotrophProcessor::HeterotrophProcessor( ) {

}

HeterotrophProcessor::~HeterotrophProcessor( ) {

}

double HeterotrophProcessor::CalculatePreferenceForPrey( const double grazerVolume, const double preyVolume ) const {

    double preference = 0;

    if( Parameters::Get( )->GetPreferenceFunctionType( ) == Constants::eNormal ) {
        preference = CalculateNormalPreferenceForPrey( grazerVolume, preyVolume );
    } else if( Parameters::Get( )->GetPreferenceFunctionType( ) == Constants::eParabolic ) {
        preference = CalculateParabolicPreferenceForPrey( grazerVolume, preyVolume );
    }

    return preference;
}

double HeterotrophProcessor::CalculateNormalPreferenceForPrey( const double grazerVolume, const double preyVolume ) const {

    double preference = Parameters::Get( )->GetPreferenceFunctionHeight( ) * Maths::Get( )->Exponential( -Maths::Get( )->ToThePower( ( Maths::Get( )->Log( ( Parameters::Get( )->GetPreferredPreyVolumeRatio( ) * preyVolume ) / grazerVolume ) ), 2 ) / ( 2 * Maths::Get( )->ToThePower( Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) ) );

    return preference;
}

double HeterotrophProcessor::CalculateParabolicPreferenceForPrey( const double grazerVolume, const double preyVolume ) const {

    double preference = Maths::Get( )->Max( 0, Parameters::Get( )->GetPreferenceFunctionHeight( ) * 1 - Maths::Get( )->ToThePower( ( Maths::Get( )->Log10( ( grazerVolume / Parameters::Get( )->GetPreferredPreyVolumeRatio( ) ) / preyVolume ) ) / Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) );

    return preference;
}

double HeterotrophProcessor::CalculateFeedingProbability( const double effectivePreyVolume ) {

    double probability = 0;

    if( Parameters::Get( )->GetFunctionalResponseType( ) == Constants::eType1 ) {
        probability = CalculateFeedingProbabilityType1( effectivePreyVolume );
    } else if( Parameters::Get( )->GetFunctionalResponseType( ) == Constants::eType2 ) {
        probability = CalculateFeedingProbabilityType2( effectivePreyVolume );
    }

    return probability;
}

double HeterotrophProcessor::CalculateMetabolicDeduction( const Types::IndividualPointer individual ) const {

    double metabolicDeduction = Parameters::Get( )->GetFractionalMetabolicExpensePerTimeStep( ) * Maths::Get( )->ToThePower( individual->GetVolumeActual( ), Parameters::Get( )->GetMetabolicIndex( ) );

    return metabolicDeduction;
}

double HeterotrophProcessor::CalculateStarvationProbability( const Types::IndividualPointer individual ) const {

    double starvationProbability = 1;

    if( Parameters::Get( )->GetStarvationFunctionType( ) == Constants::eBetaExponential ) {
        starvationProbability = CalculateBetaExponentialStarvation( individual->GetVolumeActual( ), individual->GetVolumeHeritable( ), individual->GetVolumeMinimum( ) );
    } else if( Parameters::Get( )->GetStarvationFunctionType( ) == Constants::eLinear ) {
        starvationProbability = CalculateLinearStarvation( individual->GetVolumeActual( ), individual->GetVolumeHeritable( ), individual->GetVolumeMinimum( ) );
    }
    return starvationProbability;
}

unsigned int HeterotrophProcessor::FindAndSetSizeClassIndex( const Types::IndividualPointer individual ) const {

    unsigned int sizeClassIndex = FindSizeClassIndexFromVolume( individual->GetVolumeActual( ) );

    individual->SetSizeClassIndex( sizeClassIndex );

    return sizeClassIndex;
}

unsigned int HeterotrophProcessor::FindSizeClassIndexFromVolume( const double volume ) const {

    unsigned int sizeClassIndex = 0;

    unsigned int numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );

    if( volume >= Parameters::Get( )->GetSizeClassBoundary( numberOfSizeClasses - 1 ) ) {
        sizeClassIndex = numberOfSizeClasses - 1;
    } else {
        for( unsigned int potentialIndex = 1; potentialIndex <= numberOfSizeClasses; ++potentialIndex ) {

            // Calculate and set size class index.
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

    unsigned int sizeClassIndex = individual->GetSizeClassIndex( );
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

    double probability = effectivePreyVolume / Parameters::Get( )->GetTotalVolume( );

    return probability;
}

double HeterotrophProcessor::CalculateFeedingProbabilityType2( const double effectivePreyVolume ) const {

    double probability = effectivePreyVolume / ( Parameters::Get( )->GetHalfSaturationConstant( ) + effectivePreyVolume );

    return probability;

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
        starvationProbability = 1 - ( 1 + ( ( volumeActual - volumeMinimum ) - ( volumeActual - volumeMinimum ) ) / ( ( volumeActual - volumeMinimum ) - ( ( volumeActual - volumeMinimum ) / 2 ) ) ) * Maths::Get( )->ToThePower( ( ( volumeActual - volumeMinimum ) / ( volumeActual - volumeMinimum ) ), ( ( volumeActual - volumeMinimum ) / ( ( volumeActual - volumeMinimum ) - ( ( volumeActual - volumeMinimum ) / 2 ) ) ) );
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
