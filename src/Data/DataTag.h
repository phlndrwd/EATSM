#ifndef DATATAG
#define	DATATAG

#include "Types.h"

class DataTag {
public:
    DataTag( const long, Types::IndividualPointer );
    ~DataTag( );

    void RecordTimeSeriesData( );
    void AddConsumptionEvent( const double, const double, const double, const bool );

    long GetID( ) const;
    Types::FloatMap& GetAttributes( );
    Types::FloatVectorMap& GetTimeSeriesData( );
    Types::ConsumptionEventVector& GetHerbivoryEvents( );
    Types::ConsumptionEventVector& GetCarnivoryEvents( );

private:
    Types::IndividualPointer mIndividual;
    Types::FloatMap mAttributes;
    Types::FloatVectorMap mTimeSeriesData;

    Types::ConsumptionEventVector mHerbivoryEvents;
    Types::ConsumptionEventVector mCarnivoryEvents;
    long mID;
};

#endif

