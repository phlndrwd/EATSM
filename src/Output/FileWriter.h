#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteOutputData( Types::EnvironmentPointer );

private:
    void InitialiseOutputDirectory( );
    bool WriteInputFiles( );
    
    bool WriteVectorDatums( );
    bool WriteMatrixDatums( );
    bool WriteStateFile( Types::EnvironmentPointer );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

