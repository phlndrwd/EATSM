#include "DataTag.h"
#include "Types.h"
#include "Individual.h"
#include "Time.h"

DataTag::DataTag( const Types::IndividualPointer individual, const long id ) {
    mID = id;
    
    mAttributes.insert( std::pair< std::string, float >( "VolumeHeritable", individual->GetVolumeHeritable( ) ) );
    mAttributes.insert( std::pair< std::string, float >( "VolumeMinimum", individual->GetVolumeMinimum( ) ) );
    mAttributes.insert( std::pair< std::string, float >( "VolumeReproduction", individual->GetVolumeReproduction( ) ) );

    mDataPointers.insert( std::pair< std::string, double* >( "TimeSteps", Time::Get( )->GetTimeStepPointer( ) ) );
    mDataPointers.insert( std::pair< std::string, double* >( "VolumeActual", individual->GetVolumeActualPointer( ) ) );
    mDataPointers.insert( std::pair< std::string, double* >( "TrophicLevel", individual->GetTrophicLevelPointer( ) ) );
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

void DataTag::SetData( ) {
    for( Types::DoublePointerMap::iterator pointerIter = mDataPointers.begin( ); pointerIter != mDataPointers.end( ); ++pointerIter ) {
        std::string name = pointerIter->first;
        double* dataPointer = pointerIter->second;

        Types::FloatVectorMap::iterator dataIter = mData.find( name );

        if( dataIter != mData.end( ) ) {
            dataIter->second.push_back( *dataPointer );
        } else {
            Types::FloatVector dataVector;
            dataVector.push_back( *dataPointer );
            mData.insert( std::pair< std::string, Types::FloatVector >( name, dataVector ) );
        }
    }
}