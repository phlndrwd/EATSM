#include "Autotrophs.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"

Autotrophs::Autotrophs( Types::NutrientPointer nutrient ) {
    mNutrient = nutrient;
    if( Parameters::Get( )->GetInitialisationMethod( ) == false )
        mVolume = InitialState::Get( )->GetAutotrophVolume( );
    else
        mVolume = Parameters::Get( )->GetInitialAutotrophVolume( );
    Logger::Get( )->LogMessage( "Autotroph pool created." );
}

Autotrophs::~Autotrophs( ) {
}

void Autotrophs::RecordData( ) {
    DataRecorder::Get( )->AddDataTo( "AutotrophVolume", mVolume );
    DataRecorder::Get( )->AddDataTo( "ToAutotrophFlux", mToFlux );
    mToFlux = 0;
}

void Autotrophs::Update( ) {
    double nutrientVolume = mNutrient->GetVolume( );
    double maximumPhytoplanktonVolume = Parameters::Get( )->GetInitialAutotrophVolume( );

    if( mVolume < maximumPhytoplanktonVolume ) {
        double growthVolume = maximumPhytoplanktonVolume - mVolume;

        if( growthVolume > nutrientVolume ) {
            growthVolume = nutrientVolume;
        }

        AddToVolume( growthVolume );
        mNutrient->SubtractFromVolume( growthVolume );
    }
}

double Autotrophs::GetVolume( ) {
    return mVolume;
}

void Autotrophs::SetVolume( const double volume ) {
    mVolume = volume;
}

void Autotrophs::AddToVolume( const double volume ) {
    mVolume += volume;
    mToFlux += volume;
}

void Autotrophs::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}