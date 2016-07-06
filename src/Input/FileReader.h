#ifndef FILEREADER
#define	FILEREADER

#include "Types.h"
#include "Constants.h"

class FileReader {
public:

    FileReader( );
    ~FileReader( );

    Constants::eReturnCodes ReadParametersFile( const std::string );
    Constants::eReturnCodes Restart( const std::string );
    Constants::eReturnCodes ReadStateFile( );

    const std::string GetParametersFilePath( ) const;

private:

    void SetParameter( const double );

    std::string mParametersFilePath;
    std::string mStateFilePath;
    std::string mRestartPath;

    unsigned int mParameterIndex;
};

#endif

