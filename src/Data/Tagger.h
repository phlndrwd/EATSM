#ifndef TAGGER
#define	TAGGER

#include "Types.h"

class Tagger {
public:
    Tagger( );
    ~Tagger( );

    unsigned GetNumberOfTags( );
    Types::DataTagPointer GetTag( const unsigned );
    void AllocateTag( Types::IndividualPointer );
    void RecordData( );

private:
    long mNextID;
    Types::DataTagVector mTags;
};

#endif

