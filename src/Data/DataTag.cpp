#include "DataTag.h"
#include "Types.h"
#include "Individual.h"
#include "Time.h"
#include "ConsumptionEvent.h"
#include "Logger.h"
#include "Convertor.h"

DataTag::DataTag( const long id, Types::IndividualPointer individual ) {
    mID = id;

    //    mAttributes.insert( std::pair< std::string, float >( "VolumeHeritable", individual->GetVolumeHeritable( ) ) );
    //    mAttributes.insert( std::pair< std::string, float >( "VolumeMinimum", individual->GetVolumeMinimum( ) ) );
    //    mAttributes.insert( std::pair< std::string, float >( "VolumeReproduction", individual->GetVolumeReproduction( ) ) );
    //
    //    mDataPointers.insert( std::pair< std::string, double* >( "TimeSteps", Time::Get( )->GetTimeStepPointer( ) ) );
    //    mDataPointers.insert( std::pair< std::string, double* >( "VolumeActual", individual->GetVolumeActualPointer( ) ) );
    //    mDataPointers.insert( std::pair< std::string, double* >( "TrophicLevel", individual->GetTrophicLevelPointer( ) ) );
    
    
    // CHANGE DATA COLLECTION TO WORK WITH A POINTER TO THE INDIVIDUAL!
    
    mAttributes.insert( std::make_pair( "VolumeHeritable", individual->GetVolumeHeritable( ) ) );
    mAttributes.insert( std::make_pair( "VolumeMinimum", individual->GetVolumeMinimum( ) ) );
    mAttributes.insert( std::make_pair( "VolumeReproduction", individual->GetVolumeReproduction( ) ) );

    mDataPointers.insert( std::make_pair( "TimeSteps", Time::Get( )->GetTimeStepPointer( ) ) );
    mDataPointers.insert( std::make_pair( "VolumeActual", individual->GetVolumeActualPointer( ) ) );
    mDataPointers.insert( std::make_pair( "TrophicLevel", individual->GetTrophicLevelPointer( ) ) );
}

DataTag::~DataTag( ) {

}

long DataTag::GetID( ) const {
    return mID;
}

Types::FloatMap& DataTag::GetAttributes( ) {
    return mAttributes;
}

Types::FloatVectorMap& DataTag::GetData( ) {
    return mData;
}

void DataTag::RecordTimeSeriesData( ) {
    for( Types::DoublePointerMap::iterator pointerIter = mDataPointers.begin( ); pointerIter != mDataPointers.end( ); ++pointerIter ) {
        std::string name = pointerIter->first;
        double* dataPointer = pointerIter->second;

        mData[ name ].push_back( *dataPointer );
    }
}

void DataTag::AddConsumptionEvent( const double preyVolume, const double volumeAssimilated, const double wasteVolume, const bool isHerbivory ) {
    Types::ConsumptionEventPointer consumption = new ConsumptionEvent( Time::Get( )->GetTimeStep( ), preyVolume, volumeAssimilated, wasteVolume );
    if( isHerbivory == true ) mHerbivoryEvents.push_back( consumption );
    else mCarnivoryEvents.push_back( consumption );
}