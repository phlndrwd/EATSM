#include "DataTag.h"
#include "Types.h"
#include "Individual.h"
#include "Time.h"

DataTag::DataTag( const Types::IndividualPointer individual, const long id ) {
    mIndividual = individual;
    mID = id;
    mTime.push_back( Time::Get( )->GetTimeStep( ) );
    mVolume.push_back( mIndividual->GetVolumeActual( ) );
}

DataTag::~DataTag( ) {

}

long DataTag::GetID( ) const {
    return mID;
}

unsigned DataTag::GetSize( ) const {
    return mVolume.size( );
}

float DataTag::GetVolume( const unsigned dataIndex ) const {
    return mVolume[ dataIndex ];
}

void DataTag::SetData( ) {
    mTime.push_back( Time::Get( )->GetTimeStep( ) );
    mVolume.push_back( mIndividual->GetVolumeActual( ) );
}