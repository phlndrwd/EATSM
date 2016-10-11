#ifndef FILEREADER
#define	FILEREADER

#include "Types.h"
#include "Constants.h"

class FileReader {
public:

    FileReader( );
    ~FileReader( );
    
    bool ReadInputFiles( );
    Types::StringMatrix& GetRawTextData( );

private:
    bool ReadTextFile( const std::string&, bool usesHeader = true );
    void ClearRawTextData( );

    Types::StringMatrix mRawTextData;
};

#endif

