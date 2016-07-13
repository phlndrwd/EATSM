#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteFiles( );

private:
    void InitialiseOutputDirectory( );
    void WriteInputFiles( );
    void WriteOutputData( );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

