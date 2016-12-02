#include "DataTag.h"
#include "Types.h"
#include "Individual.h"
#include "TimeStep.h"
#include "ConsumptionEvent.h"
#include "Logger.h"
#include "Convertor.h"

DataTag::DataTag( const long id, Types::IndividualPointer individual ) {
    mID = id;

    mIndividual = individual;

    mData[ "TimeSteps" ] = Types::FloatVector( );
    mData[ "VolumeActual" ] = Types::FloatVector( );
    mData[ "TrophicLevel" ] = Types::FloatVector( );

    RecordTimeSeriesData( );

    mAttributes.insert( std::pair< std::string, float >( "VolumeHeritable", individual->GetVolumeHeritable( ) ) );
    mAttributes.insert( std::pair< std::string, float >( "VolumeMinimum", individual->GetVolumeMinimum( ) ) );
    mAttributes.insert( std::pair< std::string, float >( "VolumeReproduction", individual->GetVolumeReproduction( ) ) );
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
    mData[ "TimeSteps" ].push_back( TimeStep::Get( )->GetTimeStep( ) );
    mData[ "VolumeActual" ].push_back( mIndividual->GetVolumeActual( ) );
    mData[ "TrophicLevel" ].push_back( mIndividual->GetTrophicLevel( ) );
}

void DataTag::AddConsumptionEvent( const double preyVolume, const double volumeAssimilated, const double wasteVolume, const bool isHerbivory ) {
    Types::ConsumptionEventPointer consumption = new ConsumptionEvent( TimeStep::Get( )->GetTimeStep( ), preyVolume, volumeAssimilated, wasteVolume );
    if( isHerbivory == true ) mHerbivoryEvents.push_back( consumption );
    else mCarnivoryEvents.push_back( consumption );
}
