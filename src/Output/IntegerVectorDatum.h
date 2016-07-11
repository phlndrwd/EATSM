#ifndef INTEGERVECTORDATUM
#define INTEGERVECTORDATUM

#include "Datum.h"
#include "Types.h"

class IntegerVectorDatum : public Datum {
public:
    IntegerVectorDatum( const std::string& );
    IntegerVectorDatum( unsigned, const std::string& );
    ~IntegerVectorDatum( );
    
    void SetType( );

    void AddData( const int );
    int GetData( const int ) const;
    unsigned GetLength( ) const;

private:
    Types::IntegerVector mData;
};

#endif

