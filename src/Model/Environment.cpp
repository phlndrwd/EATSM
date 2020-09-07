#include "Environment.h"
#include "Strings.h"

Environment::Environment( ) :
mNutrient( ), mAutotrophs( mNutrient ), mHeterotrophs( mNutrient, mAutotrophs ) {
    //mNutrient = new Nutrient( );
    //mAutotrophs = new Autotrophs( mNutrient );
    //mHeterotrophs = new Heterotrophs( mNutrient, mAutotrophs );
    std::cout << "Environment created." << std::endl << std::endl;
}

Environment::~Environment( ) {
    
}

void Environment::Update( ) {
    mAutotrophs.Update( );
    mHeterotrophs.Update( );
}

bool Environment::RecordData( ) {
    bool isAlive = mHeterotrophs.RecordData( );
    mAutotrophs.RecordData( );
    mNutrient.RecordData( );

    return isAlive;
}

Nutrient& Environment::GetNutrient( ) {
    return mNutrient;
}

Autotrophs&  Environment::GetAutotrophs( ) {
    return mAutotrophs;
}

Heterotrophs& Environment::GetHeterotrophs( ) {
    return mHeterotrophs;
}