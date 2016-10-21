#include "Tagger.h"

#include "DataTag.h"
#include "Individual.h"
#include "Parameters.h"

Tagger::Tagger( ) {
    mNextID = 0;
}

Tagger::~Tagger( ) {
    for( unsigned index = 0; index < mTags.size( ); ++index ) {
        delete mTags[ index ];
    }
    mTags.clear( );
}

unsigned Tagger::GetNumberOfTags( ) {
    return mTags.size( );
}

Types::DataTagPointer Tagger::GetTag( const unsigned index ) {
    return mTags[ index ];
}

void Tagger::AllocateTag( Types::IndividualPointer individual ) {
    Types::DataTagPointer tag = new DataTag( individual, mNextID );
    mTags.push_back( tag );
    ++mNextID;
}

void Tagger::RecordData( ) {
    for( unsigned index = 0; index < mTags.size( ); ++index ) {
        mTags[ index ]->SetData( );
    }
}