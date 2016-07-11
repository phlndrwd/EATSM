#include "Environment.h"

#include "Nutrient.h"
#include "Autotroph.h"
#include "Heterotrophs.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"
#include "Convertor.h"

Environment::Environment( ) {
    mNutrient = new Nutrient( );
    mPhytoplankton = new Autotroph( mNutrient );
    mHeterotrophs = new Heterotrophs( mNutrient, mPhytoplankton );
    Logger::Get( )->LogMessage( "Environment created." );
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