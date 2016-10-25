#ifndef DATATAG
#define	DATATAG

#include "Types.h"

class DataTag {
public:
    DataTag( const long, Types::IndividualPointer );
    ~DataTag( );
    
    void RecordData( );

    long GetID( ) const;
    Types::FloatMap& GetAttributes( );
    Types::FloatVectorMap& GetData( );
    
private:
    Types::FloatMap mAttributes;
    Types::DoublePointerMap mDataPointers;
    Types::FloatVectorMap mData;
    long mID;
};

#endif

