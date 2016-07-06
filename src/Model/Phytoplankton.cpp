#include "Phytoplankton.h"

#include "Nutrient.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"
#include "RestartState.h"

Phytoplankton::Phytoplankton( Types::NutrientPointer nutrient ) {

    mNutrient = nutrient;

    if( RestartState::Get( )->IsRestart( ) == true ) {
        mVolume = RestartState::Get( )->GetPhytoplanktonVolume( );
    } else {
        mVolume = Parameters::Get( )->GetInitialPhytoplanktonVolume( );
    }

    Logger::Get( )->LogString( "Phytoplankton pool created." );
}

Phytoplankton::~Phytoplankton( ) {

}

void Phytoplankton::RecordData( ) {
    DataRecorder::Get( )->AddFloatVectorData( Constants::ePhytoplanktonVolume, Constants::cVectorDatumNames[ Constants::ePhytoplanktonVolume ], mVolume );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eToPhytoplanktonFlux, Constants::cVectorDatumNames[ Constants::eToPhytoplanktonFlux ], mToFlux );

    mToFlux = 0;
}

void Phytoplankton::Update( ) {

    double nutrientVolume = mNutrient->GetVolume( );
    double maximumPhytoplanktonVolume = Parameters::Get( )->GetInitialPhytoplanktonVolume( );


    if( mVolume < maximumPhytoplanktonVolume ) {
        double growthVolume = maximumPhytoplanktonVolume - mVolume;

        if( growthVolume > nutrientVolume ) {
            growthVolume = nutrientVolume;
        }

        AddToVolume( growthVolume );
        mNutrient->SubtractFromVolume( growthVolume );
    }
}

double Phytoplankton::GetVolume( ) {
    return mVolume;
}

void Phytoplankton::SetVolume( const double volume ) {
    mVolume = volume;
}

void Phytoplankton::AddToVolume( const double volume ) {
    mVolume += volume;
    mToFlux += volume;
}

void Phytoplankton::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}
