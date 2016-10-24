#ifndef DATATAG
#define	DATATAG

#include "Types.h"

class DataTag {
public:
    DataTag( const Types::IndividualPointer, const long );
    ~DataTag( );

    long GetID( ) const;
    Types::FloatMap& GetAttributes( );
    Types::FloatVectorMap& GetData( );
    void SetData( );

private:
    Types::FloatMap mAttributes;
    Types::DoublePointerMap mDataPointers;
    Types::FloatVectorMap mData;
    long mID;
};

#endif

