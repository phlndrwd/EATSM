#include "Autotrophs.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "DataRecorder.h"
#include "Parameters.h"

#include <iostream>

Autotrophs::Autotrophs( ) {

}

Autotrophs::Autotrophs( Types::NutrientPointer nutrient ) {
    mNutrient = nutrient;
    if( InitialState::Get( )->IsInitialised( ) == true ) mVolume = InitialState::Get( )->GetAutotrophVolume( );
    else mVolume = Parameters::Get( )->GetInitialAutotrophVolume( );
    mMaximumVolume = Parameters::Get( )->GetInitialAutotrophVolume( );

    std::cout << "Autotroph pool created." << std::endl;
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

    if( mVolume < mMaximumVolume ) {
        double growthVolume = mMaximumVolume - mVolume;
        if( growthVolume > nutrientVolume ) growthVolume = nutrientVolume;

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
