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

Types::DataTagPointer Tagger::GetNextTag( Types::IndividualPointer individual  ) {
    Types::DataTagPointer tag = new DataTag( mNextID, individual );
    mTags.push_back( tag );
    ++mNextID;
    return tag;
}
