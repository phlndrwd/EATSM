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
    void WriteInputFiles( );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

