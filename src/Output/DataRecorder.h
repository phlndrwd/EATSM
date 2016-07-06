#ifndef DATARECORDER
#define	DATARECORDER

#include "Types.h"
#include "Constants.h"

class DataRecorder {
public:
    ~DataRecorder( );

    static Types::DataRecorderPointer Get( );

    void AddIntegerVectorData( const unsigned int, const std::string, const int );
    void AddIntegerMatrixData( const unsigned int, const std::string, const Types::IntegerVector );

    void AddFloatVectorData( const unsigned int, const std::string, const float );
    void AddFloatVectorData( const unsigned int, const std::string, const Types::FloatVector );
    void AddFloatMatrixData( const unsigned int, const std::string, const Types::FloatVector );

    unsigned int GetNumberOfDatums( ) const;
    Types::DatumPointer GetDatumPointerFromIndex( const unsigned int ) const;

private:
    DataRecorder( );

    int GetDatumIndexFromName( const unsigned int, const std::string, const Constants::eDatumTypes );

    static Types::DataRecorderPointer mThis;

    Types::DatumArray mDatums;
};

#endif
