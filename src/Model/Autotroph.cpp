#include "Autotroph.h"

#include "Nutrient.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"

Autotroph::Autotroph( Types::NutrientPointer nutrient ) {
    mNutrient = nutrient;
    mVolume = Parameters::Get( )->GetInitialAutotrophVolume( );
    Logger::Get( )->LogMessage( "Phytoplankton pool created." );
}

Autotroph::~Autotroph( ) {

}

void Autotroph::RecordData( ) {
    DataRecorder::Get( )->AddFloatVectorData( Constants::eAutotrophVolume, Constants::cVectorDatumNames[ Constants::eAutotrophVolume ], mVolume );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eToPhytoplanktonFlux, Constants::cVectorDatumNames[ Constants::eToPhytoplanktonFlux ], mToFlux );

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
