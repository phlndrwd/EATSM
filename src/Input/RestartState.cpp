#include "RestartState.h"

#include "Parameters.h"
#include "Individual.h"

Types::RestartStatePointer RestartState::mThis = 0;

Types::RestartStatePointer RestartState::Get( ) {
    if( mThis == 0 ) {
        mThis = new RestartState( );
    }
    return mThis;
}

RestartState::~RestartState( ) {
    if( mThis != 0 ) {
        for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
            for( unsigned int individualIndex = 0; individualIndex < mSizeClasses[ sizeClassIndex ].size( ); ++individualIndex ) {
                delete mSizeClasses[ sizeClassIndex ][ individualIndex ];
            }
            mSizeClasses[ sizeClassIndex ].clear( );
        }
        mSizeClasses.clear( );
        mRestartPath.clear( );

        delete mThis;
    }
}

RestartState::RestartState( ) {
    mIsRestart = false;
    mPhytoplanktonVolume = 0;
    mStartingTimeStep = 0;
    mHeterotrophFrequency = 0;

    mSizeClasses.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
}

void RestartState::AddIndividual( const Types::IndividualPointer individual ) {
    mSizeClasses[ individual->GetSizeClassIndex( ) ].push_back( individual );
    ++mHeterotrophFrequency;
}

bool RestartState::IsRestart( ) const {
    return mIsRestart;
}

unsigned int RestartState::GetStartingTimeStep( ) const {
    return mStartingTimeStep;
}

double RestartState::GetNutrientVolume( ) const {
    return mNutrientVolume;
}

double RestartState::GetPhytoplanktonVolume( ) const {
    return mPhytoplanktonVolume;
}

const std::string RestartState::GetRestartPath( ) const {
    return mRestartPath;
}

const Types::IndividualMatrix& RestartState::GetSizeClasses( ) const {
    return mSizeClasses;
}

unsigned int RestartState::GetHeterotrophFrequency( ) const {
    return mHeterotrophFrequency;
}

void RestartState::SetIsRestart( const bool isRestart ) {
    mIsRestart = isRestart;
}

void RestartState::SetStartingTimeStep( const unsigned int startingTimeStep ) {
    mStartingTimeStep = startingTimeStep;
}

void RestartState::SetNutrientVolume( const double nutrientVolume ) {
    mNutrientVolume = nutrientVolume;
}

void RestartState::SetPhytoplanktonVolume( const double phytoplanktonVolume ) {
    mPhytoplanktonVolume = phytoplanktonVolume;
}

void RestartState::SetRestartPath( const std::string restartPath ) {
    mRestartPath = restartPath;
}

void RestartState::SetSizeClasses( const Types::IndividualMatrix individualMatrix ) {
    mSizeClasses = individualMatrix;
}
