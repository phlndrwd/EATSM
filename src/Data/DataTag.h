#ifndef DATATAG
#define	DATATAG

#include "Types.h"

class DataTag {
public:
    DataTag( const Types::IndividualPointer, const long );
    ~DataTag( );

    long GetID( ) const;
    unsigned GetSize( ) const;
    float GetVolume( const unsigned ) const;
    void SetData( );

private:
    Types::IndividualPointer mIndividual;
    Types::UnsignedVector mTime;
    Types::FloatVector mVolume;
    long mID;
};

#endif

