#include "InitialState.h"
#include "Strings.h"
#include "Individual.h"
#include "Parameters.h"
#include "HeterotrophProcessor.h"

Types::InitialStatePointer InitialState::mThis = NULL;

Types::InitialStatePointer InitialState::Get( ) {
    if( mThis == NULL ) mThis = new InitialState( );

    return mThis;
}

InitialState::~InitialState( ) {
    if( mThis != NULL ) delete mThis;
}

InitialState::InitialState( ) {

}

bool InitialState::Initialise( const Types::StringMatrix& rawInitialStateData ) {
    // Model variables
    mNutrientVolume = Strings::StringToNumber( rawInitialStateData[ Constants::cStateLineNutrientVol ][ 0 ] );
    mAutotrophVolume = Strings::StringToNumber( rawInitialStateData[ Constants::cStateLineAutotrophVol ][ 0 ] );
    // Heterotrophs
    mInitialPopulationSize = 0;
    // TODO - Restart uses a mixture of restart and parameters files as input.
    mHeterotrophs.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    for( unsigned lineIndex = Constants::cStateLineFirstHeterotroph; lineIndex < rawInitialStateData.size( ); ++lineIndex ) {
        double geneValue = Strings::StringToNumber( rawInitialStateData[ lineIndex ][ 0 ] );
        double volumeActual = Strings::StringToNumber( rawInitialStateData[ lineIndex ][ 1 ] );
        unsigned sizeClassIndex = Strings::StringToNumber( rawInitialStateData[ lineIndex ][ 2 ] );

        double volumeHeritable = HeterotrophProcessor::TraitValueToVolume( geneValue );
        Types::IndividualPointer individual = new Individual( geneValue, volumeHeritable, volumeActual, sizeClassIndex );
        mHeterotrophs[ sizeClassIndex ].push_back( individual );
        ++mInitialPopulationSize;
    }
    return true; // Currently no circumstance under which this function can return false.
}

double& InitialState::GetNutrientVolume( ) {
    return mNutrientVolume;
}

double& InitialState::GetAutotrophVolume( ) {
    return mAutotrophVolume;
}

Types::IndividualMatrix& InitialState::GetHeterotrophs( ) {
    return mHeterotrophs;
}

unsigned& InitialState::GetInitialPopulationSize( ) {
    return mInitialPopulationSize;
}
