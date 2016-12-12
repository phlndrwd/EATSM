#include "InitialState.h"
#include "StringTools.h"
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
    // Essential call to RandomSFMT to construct the static object and allow state initialisation to overwrite constructed values.
    RandomSFMT::Get( );

    bool success = true;

    // Calculated normal data
    bool isNormalValueCalculated = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineIsNormalCalculated ][ 0 ] );
    RandomSFMT::Get( )->SetIsNormalCalculated( isNormalValueCalculated );

    double calculatedNormalValue = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineNormalValue ][ 0 ] );
    RandomSFMT::Get( )->SetCalculatedNormalValue( calculatedNormalValue );

    // Random state index
    unsigned stateIndex = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineRandomIndex ][ 0 ] );
    RandomSFMT::Get( )->SetStateIndex( stateIndex );

    // Random (mother-of-all) state
    unsigned randomMotherState[ MOA_N ];
    for( unsigned columnIndex = 0; columnIndex < MOA_N; ++columnIndex ) {
        randomMotherState[ columnIndex ] = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineMOAState ][ columnIndex ] );
    }
    RandomSFMT::Get( )->SetMotherState( randomMotherState );

    // Random (SFMT) state
    unsigned arrayCounter = 0;
    __m128i randomSFMTState[ SFMT_N ];
    for( unsigned lineIndex = Constants::cStateLineSFMTState; lineIndex < SFMT_N + Constants::cStateLineSFMTState; ++lineIndex ) {
        unsigned values[ 4 ];
        for( unsigned columnIndex = 0; columnIndex < 4; ++columnIndex ) {
            values[ columnIndex ] = StringTools::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ columnIndex ] );
        }
        randomSFMTState[ arrayCounter ] = _mm_loadu_si128( ( __m128i* )values );
        ++arrayCounter;
    }
    RandomSFMT::Get( )->SetState( randomSFMTState );

    // Model variables
    mNutrientVolume = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineNutrientVol ][ 0 ] );
    mAutotrophVolume = StringTools::Get( )->StringToNumber( rawInitialStateData[ Constants::cStateLineAutotrophVol ][ 0 ] );
    // Heterotrophs
    mInitialPopulationSize = 0;
    mHeterotrophs.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    HeterotrophProcessor heterotrophProcessor;
    for( unsigned lineIndex = Constants::cStateLineFirstHeterotroph; lineIndex < rawInitialStateData.size( ); ++lineIndex ) {
        unsigned sizeClassIndex = StringTools::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 0 ] );
        double geneValue = StringTools::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 1 ] );
        double volumeActual = StringTools::Get( )->StringToNumber( rawInitialStateData[ lineIndex ][ 2 ] );

        Types::IndividualPointer individual = new Individual( &heterotrophProcessor, geneValue, volumeActual, sizeClassIndex );
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
