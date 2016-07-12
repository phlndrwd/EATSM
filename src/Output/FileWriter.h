#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteFiles( );

    const std::string GetOutputPath( );

private:
    void InitialiseOutputDirectory( );
    void WriteOutputData( );
    void WriteInputFiles( );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

