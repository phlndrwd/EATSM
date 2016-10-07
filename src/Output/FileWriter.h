#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteOutputData( );

private:
    void InitialiseOutputDirectory( );
    void WriteInputFiles( );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

