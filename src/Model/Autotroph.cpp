#include "Autotroph.h"

#include "Nutrient.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"

Autotroph::Autotroph( Types::NutrientPointer nutrient ) {
    mNutrient = nutrient;
    mVolume = Parameters::Get( )->GetInitialAutotrophVolume( );
    Logger::Get( )->LogMessage( "Autotroph pool created." );
}

Autotroph::~Autotroph( ) {

}

void Autotroph::RecordData( ) {
    DataRecorder::Get( )->AddDataTo( "AutotrophVolume", mVolume );
    DataRecorder::Get( )->AddDataTo( "ToAutotrophFlux", mToFlux );
    mToFlux = 0;
}

void Autotroph::Update( ) {
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

double Autotroph::GetVolume( ) {
    return mVolume;
}

void Autotroph::SetVolume( const double volume ) {
    mVolume = volume;
}

void Autotroph::AddToVolume( const double volume ) {
    mVolume += volume;
    mToFlux += volume;
}

void Autotroph::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}
