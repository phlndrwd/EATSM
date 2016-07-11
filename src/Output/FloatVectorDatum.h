#ifndef FLOATVECTORDATUM
#define	FLOATVECTORDATUM

#include "Datum.h"
#include "Types.h"

class FloatVectorDatum : public Datum {
public:
    FloatVectorDatum( const std::string& );
    FloatVectorDatum( const unsigned, const std::string& );
    ~FloatVectorDatum( );
    
    void SetType( );

    void AddData( const float );
    float GetData( const int ) const;
    unsigned GetLength( ) const;

private:
    Types::FloatVector mData;
};

#endif

