#include "Nutrient.h"
#include "InitialState.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Constants.h"

#include <iostream>

Nutrient::Nutrient( ) {
    if( Parameters::Get( )->GetCreateNewPopulation( ) == false )
        mVolume = InitialState::Get( )->GetNutrientVolume( );
    else
        mVolume = Parameters::Get( )->GetInitialNutrientVolume( );
    
    std::cout << "Nutrient pool created." << std::endl;
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

void Nutrient::SubtractFromVolume( const double volume ) {
    mVolume -= volume;
}
