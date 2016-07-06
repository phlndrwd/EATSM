#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:
    ~FileWriter( );

    static Types::FileWriterPointer Get( );

    void InitialiseOutputPath( );
    void WriteOutputDataToFile( Types::EnvironmentPointer, const unsigned int, const unsigned int, const bool );

    const std::string GetOutputPath( );
    void SetDataSetDirectoryName( const std::string );
    
    void LogTerminalToFile( const std::string );

private:
    FileWriter( );

    void CreateParameterStringVectors( const unsigned int totalNumberOfTimeSteps = 0, const unsigned int runTimeInSeconds = 0, const bool didHeterotrophsLive = true );
    const std::string GetParameterLine( const int ) const;

    bool IsNameOfVectorDatum( const std::string ) const;
    bool IsNameOfSizeClassDatum( const std::string ) const;

    static Types::FileWriterPointer mThis;

    Types::StringVector mParameterNames;
    Types::StringVector mParameterValues;

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
    
    Types::OutputFileStream mTerminalFile;
    std::string mTerminalFilePath;

    unsigned int mWidthOfWidestParameterSet;
    unsigned int mNumberOfDataPoints;
    
    bool mDataSetDirectoryNameIsSet;
};

#endif

