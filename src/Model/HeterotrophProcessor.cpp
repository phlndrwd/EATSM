#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "Individual.h"
#include "RandomSimple.h"

#include <cmath>
#include <iostream>


HeterotrophProcessor::HeterotrophProcessor( ) {
    // Set function option once for the duration of the model execution
    if( Parameters::Get()->GetUseLinearFeeding() == true ) fStarvationProbability = &HeterotrophProcessor::CalculateFeedingProbabilityLinear;
    else fStarvationProbability = &HeterotrophProcessor::CalculateFeedingProbabilityNonLinear;
}

HeterotrophProcessor::~HeterotrophProcessor( ) {

}

double HeterotrophProcessor::CalculatePreferenceForPrey( const double grazerVolume, const double preyVolume ) const {
    return std::exp( -std::pow( ( std::log( ( Parameters::Get( )->GetPreferredPreyVolumeRatio( ) * preyVolume ) / grazerVolume ) ), 2 ) / ( 2 * std::pow( Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) ) );
}

double HeterotrophProcessor::CalculateFeedingProbability( const unsigned predatorIndex, const double effectivePreyVolume ) {
    return ( this->*fStarvationProbability )( predatorIndex, effectivePreyVolume );
}

double HeterotrophProcessor::CalculateMetabolicDeduction( const Types::IndividualPointer individual ) const {
    return Parameters::Get( )->GetFractionalMetabolicExpense( ) * std::pow( individual->GetVolumeActual( ), Parameters::Get( )->GetMetabolicIndex( ) );
}

double HeterotrophProcessor::CalculateStarvationProbability( const Types::IndividualPointer individual ) const {
    return CalculateLinearStarvation( individual->GetVolumeActual( ), individual->GetVolumeHeritable( ), individual->GetVolumeMinimum( ), individual->GetStarvationMultiplier( ) );
}

unsigned HeterotrophProcessor::FindIndividualSizeClassIndex( const Types::IndividualPointer individual, unsigned directionToMove ) const {
    unsigned currentSizeClass = individual->GetSizeClassIndex( );
    unsigned newSizeClassIndex = currentSizeClass;
    double volume = individual->GetVolumeActual( );

    if( directionToMove == Constants::eMoveUp ) {
        for( unsigned index = currentSizeClass; index < Parameters::Get( )->GetNumberOfSizeClasses( ); ++index ) {
            if( volume < Parameters::Get( )->GetSizeClassBoundary( index ) ) {
                newSizeClassIndex = index - 1;
                break;
            }
        }
    } else if( directionToMove == Constants::eMoveDown ) {
        for( int index = currentSizeClass; index >= 0; --index ) {
            if( volume >= Parameters::Get( )->GetSizeClassBoundary( index ) ) {
                newSizeClassIndex = ( unsigned )index;
                break;
            }
        }
    }

    return newSizeClassIndex;
}

bool HeterotrophProcessor::UpdateSizeClassIndex( const Types::IndividualPointer individual ) const {
    unsigned directionToMove = DirectionIndividualShouldMoveSizeClasses( individual );
    if( directionToMove != Constants::eNoMovement ) {
        unsigned newSizeClassIndex = FindIndividualSizeClassIndex( individual, directionToMove );
        individual->SetSizeClassIndex( newSizeClassIndex );
        return true;
    }
    return false;
}

unsigned HeterotrophProcessor::FindSizeClassIndexFromVolume( const double volume ) const {
    unsigned sizeClassIndex = 0;

    for( unsigned index = 1; index <= Parameters::Get( )->GetNumberOfSizeClasses( ); ++index ) {
        if( volume < Parameters::Get( )->GetSizeClassBoundary( index ) ) {
            sizeClassIndex = index - 1;
            break;
        }
    }
    return sizeClassIndex;
}

unsigned HeterotrophProcessor::DirectionIndividualShouldMoveSizeClasses( const Types::IndividualPointer individual ) const {
    unsigned directionToMove = Constants::eNoMovement;

    unsigned sizeClassIndex = individual->GetSizeClassIndex( );
    double volumeActual = individual->GetVolumeActual( );

    if( volumeActual < Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex ) )
        directionToMove = Constants::eMoveDown;
    else if( volumeActual >= Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex + 1 ) )
        directionToMove = Constants::eMoveUp;
    
    return directionToMove;
}

double HeterotrophProcessor::CalculateFeedingProbabilityLinear( const unsigned predatorIndex, const double effectivePreyVolume ) {
    return std::min( effectivePreyVolume / ( Parameters::Get( )->GetLinearFeedingDenominator( predatorIndex ) ), 1.0 );
}

double HeterotrophProcessor::CalculateFeedingProbabilityNonLinear( const unsigned predatorIndex, const double effectivePreyVolume ) {
    return ( effectivePreyVolume / ( Parameters::Get( )->GetHalfSaturationConstant( predatorIndex ) + effectivePreyVolume ) );
}

double HeterotrophProcessor::CalculateLinearStarvation( const double& volumeActual, const double& volumeHeritable, const double& volumeMinimum, const double& starvationMultiplier ) const {
    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum )
        starvationProbability = 1;
    else if( volumeActual >= volumeHeritable )
        starvationProbability = 0;
    else
        starvationProbability = 1 + ( ( volumeMinimum - volumeActual ) * starvationMultiplier );
    
    return starvationProbability;
}

double HeterotrophProcessor::CalculateBetaExponentialStarvation( const double& volumeActual, const double& volumeHeritable, const double& volumeMinimum, const double& starvationMultiplier ) const {
    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum )
        starvationProbability = 1;
    else if( volumeActual >= volumeHeritable )
        starvationProbability = 0;
    else
        starvationProbability = 1 - ( 1 + ( ( volumeHeritable - volumeMinimum ) - ( volumeActual - volumeMinimum ) ) * starvationMultiplier ) * ( ( volumeActual - volumeMinimum ) * starvationMultiplier );

    return starvationProbability;
}

double HeterotrophProcessor::TraitValueToVolume( double traitValue ) {
    double volumeExponent = traitValue * ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) + Parameters::Get( )->GetSmallestVolumeExponent( );
    return std::pow( 10, volumeExponent );
}

double HeterotrophProcessor::VolumeToTraitValue( double volume ) const {
    return ( std::log10( volume ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) / ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) );
}

int HeterotrophProcessor::RoundWithProbability( const double& value ) const {
    int flooredValue = static_cast < int >( ::floor( value ) );
    double probability = value - flooredValue;

    if( RandomSimple::Get( )->GetUniform( ) < probability ) return flooredValue + 1;
    else return flooredValue;
}
