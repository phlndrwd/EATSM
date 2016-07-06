#include "Environment.h"

#include "Nutrient.h"
#include "Phytoplankton.h"
#include "Heterotrophs.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"
#include "Convertor.h"

Environment::Environment( ) {
    mNutrient = new Nutrient( );
    mPhytoplankton = new Phytoplankton( mNutrient );
    mHeterotrophs = new Heterotrophs( mNutrient, mPhytoplankton );
    Logger::Get( )->LogString( "Environment created." );
}

Environment::~Environment( ) {
    delete mHeterotrophs;
}

void Environment::Update( ) {

    mPhytoplankton->Update( );
    mHeterotrophs->Update( );
}

bool Environment::RecordData( ) {

    bool isAlive = mHeterotrophs->RecordData( );
    mPhytoplankton->RecordData( );
    mNutrient->RecordData( );

    return isAlive;
}

void Environment::OutputHeterotrophSummaryData( ) const {
    mHeterotrophs->OutputSummaryData( );
}

Types::NutrientPointer Environment::GetNutrient( ) {
    return mNutrient;
}

Types::PhytoplanktonPointer Environment::GetPhytoplankton( ) {
    return mPhytoplankton;
}

Types::HeterotrophsPointer Environment::GetHeterotrophs( ) {
    return mHeterotrophs;
}
