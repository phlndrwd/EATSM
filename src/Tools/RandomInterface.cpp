#include "RandomInterface.h"

#include "Parameters.h"

Types::RandomInterfacePointer RandomInterface::mThis = NULL;

Types::RandomInterfacePointer RandomInterface::Get( ) {
    if( mThis == NULL ) {
        mThis = new RandomInterface( Parameters::Get( )->GetRandomSeed( ) );
    }
    return mThis;
}

RandomInterface::RandomInterface( unsigned seed ) {
    mRandomDefault = new RandomDefault( seed );
    Parameters::Get( )->SetRandomSeed( mRandomDefault->GetSeed( ) ); // Records actual seed for writing to output
}

RandomInterface::~RandomInterface( ) {
    if( mRandomDefault != NULL ) delete mRandomDefault;
}

int RandomInterface::GetUniformInt( const float minimum, const float maximum ) {
    return mRandomDefault->GetUniformInt( minimum, maximum );
}

double RandomInterface::GetUniformDouble( const float minimum, const float maximum ) {
    return mRandomDefault->GetUniformDouble( minimum, maximum );
}

double RandomInterface::GetNormal( const float mean, const float standardDeviation ) {
    return mRandomDefault->GetNormal( mean, standardDeviation );
}

unsigned RandomInterface::GetRandomSeed( ) const {
    return mRandomDefault->GetSeed( );
}