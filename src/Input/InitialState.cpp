#include "InitialState.h"
#include "Logger.h"
#include "Convertor.h"
#include "Individual.h"
#include "Parameters.h"

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
    bool success = true;

    mInitialPopulationSize = 0;
    mHeterotrophs.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    mNutrientVolume = Convertor::Get( )->StringToNumber( rawInitialStateData[ 0 ][ 0 ] );
    mAutotrophVolume = Convertor::Get( )->StringToNumber( rawInitialStateData[ 1 ][ 0 ] );

    for( unsigned lineIndex = 2; lineIndex < rawInitialStateData.size( ); ++lineIndex ) {
        unsigned sizeClassIndex = Convertor::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 0 ] );
        double geneValue = Convertor::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 1 ] );
        double volumeActual = Convertor::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 2 ] );

        Types::IndividualPointer individual = new Individual( geneValue, volumeActual, sizeClassIndex );
        mHeterotrophs[ sizeClassIndex ].push_back( individual );
        ++mInitialPopulationSize;
    }

    return success;
}

double InitialState::GetNutrientVolume( ) const {
    return mNutrientVolume;
}

double InitialState::GetAutotrophVolume( ) const {
    return mAutotrophVolume;
}

Types::IndividualMatrix& InitialState::GetHeterotrophs( ) {
    return mHeterotrophs;
}

unsigned InitialState::GetInitialPopulationSize( ) {
    return mInitialPopulationSize;
}