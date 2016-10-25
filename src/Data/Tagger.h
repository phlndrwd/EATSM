#ifndef TAGGER
#define	TAGGER

#include "Types.h"

class Tagger {
public:
    Tagger( );
    ~Tagger( );

    unsigned GetNumberOfTags( );
    Types::DataTagPointer GetTag( const unsigned );
    Types::DataTagPointer GetNextTag( Types::IndividualPointer );

private:
    long mNextID;
    Types::DataTagVector mTags;
};

#endif

