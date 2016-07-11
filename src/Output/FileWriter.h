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

    bool IsNameOfVectorDatum( const std::string ) const;
    bool IsNameOfSizeClassDatum( const std::string ) const;

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

