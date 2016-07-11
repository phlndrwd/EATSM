#ifndef FLOATMATRIXDATUM
#define	FLOATMATRIXDATUM

#include "Datum.h"
#include "Types.h"

class FloatMatrixDatum : public Datum {
public:
    FloatMatrixDatum( const std::string& );
    FloatMatrixDatum( const unsigned, const std::string& );
    ~FloatMatrixDatum( );
    
    void SetType( );

    void AddData( const Types::FloatVector );
    void AddData( const int, const int, const float );

    const Types::FloatVector GetData( const int ) const;
    float GetData( const int, const int ) const;
    
    unsigned GetRows( ) const;
    unsigned GetColumns( ) const;

private:
    Types::FloatMatrix mData;
};

#endif

