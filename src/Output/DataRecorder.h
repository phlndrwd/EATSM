#ifndef DATARECORDER
#define	DATARECORDER

#include "Types.h"
#include "Constants.h"

class DataRecorder {
public:
    ~DataRecorder( );

    static Types::DataRecorderPointer Get( );

    void AddIntegerVectorData( const unsigned, const std::string, const int );
    void AddIntegerMatrixData( const unsigned, const std::string, const Types::IntegerVector );

    void AddFloatVectorData( const unsigned, const std::string, const float );
    void AddFloatVectorData( const unsigned, const std::string, const Types::FloatVector );
    void AddFloatMatrixData( const unsigned, const std::string, const Types::FloatVector );

    unsigned GetNumberOfDatums( ) const;
    Types::DatumPointer GetDatumPointerFromIndex( const unsigned ) const;
    
    void AddInputFilePath( const std::string& );
    Types::StringVector GetInputFilePaths( ) const;

private:
    DataRecorder( );

    int GetDatumIndexFromName( const unsigned, const std::string, const Constants::eDatumTypes );

    static Types::DataRecorderPointer mThis;

    Types::DatumArray mDatums;
    
    Types::StringVector mInputFilePaths;
};

#endif
