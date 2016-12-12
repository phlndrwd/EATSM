#include "Environment.h"

#include "Nutrient.h"
#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "Strings.h"

Environment::Environment( ) {
    mNutrient = new Nutrient( );
    mAutotrophs = new Autotrophs( mNutrient );
    mHeterotrophs = new Heterotrophs( mNutrient, mAutotrophs );
    std::cout << "Environment created." << std::endl;
}

Environment::~Environment( ) {
    delete mHeterotrophs;
}

void Environment::Update( ) {
    mAutotrophs->Update( );
    mHeterotrophs->Update( );
}

bool Environment::RecordData( ) {
    bool isAlive = mHeterotrophs->RecordData( );
    mAutotrophs->RecordData( );
    mNutrient->RecordData( );

    return isAlive;
}

Types::NutrientPointer Environment::GetNutrient( ) const {
    return mNutrient;
}

Types::AutotrophsPointer Environment::GetAutotrophs( ) const {
    return mAutotrophs;
}

Types::HeterotrophsPointer Environment::GetHeterotrophs( ) const {
    return mHeterotrophs;
}