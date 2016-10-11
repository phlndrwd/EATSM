#include "Nutrient.h"
#include "InitialState.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Constants.h"
#include "Logger.h"

Nutrient::Nutrient( ) {
    if( Parameters::Get( )->GetInitialisationMethod( ) == false )
        mVolume = InitialState::Get( )->GetNutrientVolume( );
    else
        mVolume = Parameters::Get( )->GetInitialNutrientVolume( );
    
    Logger::Get( )->LogMessage( "Nutrient pool created." );
}

Nutrient::~Nutrient( ) {

}

void Nutrient::RecordData( ) {
    DataRecorder::Get( )->AddDataTo( "NutrientVolume", mVolume );
    DataRecorder::Get( )->AddDataTo( "ToNutrientFlux", mToFlux );
    mToFlux = 0;
}

double Nutrient::GetVolume( ) {
    return mVolume;
}

void Nutrient::SetVolume( const double volume ) {
    mVolume = volume;
}

void Nutrient::AddToVolume( const double volume ) {
    mVolume += volume;
    mToFlux += volume;
}

//void Nutrient::AddToVolumeNoFlux( const double volume ) {
//    mVolume += volume;
//}

void Nutrient::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}
