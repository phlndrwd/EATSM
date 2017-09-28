#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "Individual.h"
#include "RandomSFMT.h"

#include <cmath>

HeterotrophProcessor::HeterotrophProcessor( ) {

}

HeterotrophProcessor::~HeterotrophProcessor( ) {

}

double HeterotrophProcessor::CalculatePreferenceForPrey( const double grazerVolume, const double preyVolume ) const {
    return std::exp( -std::pow( ( std::log( ( Parameters::Get( )->GetPreferredPreyVolumeRatio( ) * preyVolume ) / grazerVolume ) ), 2 ) / ( 2 * std::pow( Parameters::Get( )->GetPreferenceFunctionWidth( ), 2 ) ) );
}

double HeterotrophProcessor::CalculateFeedingProbability( const double effectivePreyVolume ) {
    return CalculateFeedingProbabilityType2( effectivePreyVolume );
}

double HeterotrophProcessor::CalculateMetabolicDeduction( const Types::IndividualPointer individual ) const {
    return Parameters::Get( )->GetFractionalMetabolicExpense( ) * std::pow( individual->GetVolumeActual( ), Parameters::Get( )->GetMetabolicIndex( ) );
}

double HeterotrophProcessor::CalculateStarvationProbability( const Types::IndividualPointer individual ) const {
    return CalculateLinearStarvation( individual->GetVolumeActual( ), individual->GetVolumeHeritable( ), individual->GetVolumeMinimum( ) );
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

void HeterotrophProcessor::UpdateSizeClassIndex( const Types::IndividualPointer individual ) const {
    unsigned directionToMove = DirectionIndividualShouldMoveSizeClasses( individual );
    if( directionToMove != Constants::eNoMovement ) {
        unsigned newSizeClassIndex = FindIndividualSizeClassIndex( individual, directionToMove );
        individual->SetSizeClassIndex( newSizeClassIndex );
    }
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

    if( volumeActual >= Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex + 1 ) )
        directionToMove = Constants::eMoveUp;
    else if( volumeActual < Parameters::Get( )->GetSizeClassBoundary( sizeClassIndex ) )
        directionToMove = Constants::eMoveDown;

    return directionToMove;
}

double HeterotrophProcessor::CalculateFeedingProbabilityType1( const double effectivePreyVolume ) const {
    return ( effectivePreyVolume / Parameters::Get( )->GetTotalVolume( ) );
}

double HeterotrophProcessor::CalculateFeedingProbabilityType2( const double effectivePreyVolume ) const {
    return ( effectivePreyVolume / ( Parameters::Get( )->GetHalfSaturationConstant( ) + effectivePreyVolume ) );
}

double HeterotrophProcessor::CalculateLinearStarvation( const double volumeActual, const double volumeHeritable, const double volumeMinimum ) const {
    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum )
        starvationProbability = 1;
    else if( volumeActual >= volumeHeritable )
        starvationProbability = 0;
    else
        starvationProbability = 1 + ( ( volumeMinimum - volumeActual ) / ( volumeHeritable - volumeMinimum ) );
    
    return starvationProbability;
}

double HeterotrophProcessor::CalculateBetaExponentialStarvation( const double volumeActual, const double volumeHeritable, const double volumeMinimum ) const {
    double starvationProbability = 1;

    if( volumeActual <= volumeMinimum )
        starvationProbability = 1;
    else if( volumeActual >= volumeHeritable )
        starvationProbability = 0;
    else
        starvationProbability = 1 - ( 1 + ( ( volumeHeritable - volumeMinimum ) - ( volumeActual - volumeMinimum ) ) / ( ( volumeHeritable - volumeMinimum ) ) ) * ( ( volumeActual - volumeMinimum ) / ( volumeHeritable - volumeMinimum ) );

    return starvationProbability;
}

double HeterotrophProcessor::GeneValueToVolume( double geneValue ) const {
    double volumeExponent = geneValue * ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) + Parameters::Get( )->GetSmallestVolumeExponent( );
    double volume = std::pow( 10, volumeExponent );

    return volume;
}

double HeterotrophProcessor::VolumeToGeneValue( double volume ) const {
    double geneValue = ( std::log10( volume ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) / ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) );

    return geneValue;
}

int HeterotrophProcessor::RoundWithProbability( const double& value ) const {
    int flooredValue = static_cast < int >( ::floor( value ) );
    double probability = value - flooredValue;

    if( RandomSFMT::Get( )->GetUniform( ) < probability )
        return flooredValue + 1;
    else
        return flooredValue;
}
