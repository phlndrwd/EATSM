#ifndef INTEGERMATRIXDATUM
#define	INTEGERMATRIXDATUM

#include "Datum.h"
#include "Types.h"

class IntegerMatrixDatum : public Datum {
public:
    IntegerMatrixDatum( const std::string& );
    IntegerMatrixDatum( const unsigned, const std::string& );
    ~IntegerMatrixDatum( );
    
    void SetType( );
    
    void AddData( const Types::IntegerVector );
    void AddData( const int, const int, const int );

    const Types::IntegerVector GetData( const int ) const;
    int GetData( const int, const int ) const;
    
    unsigned GetRows( ) const;
    unsigned GetColumns( ) const;

private:
    Types::IntegerMatrix mData;
};

#endif

