#include "FileReader.h"

#include "Parameters.h"
#include "Individual.h"
#include "Logger.h"
#include "Genome.h"
#include "Convertor.h"
#include "DataRecorder.h"

FileReader::FileReader( ) {

}

FileReader::~FileReader( ) {
}

bool FileReader::ReadInputFiles( ) {
    bool success = ReadTextFile( Constants::cConfigurationDirectory + Constants::cInputParametersFileName );

    if( success == true )
        success = Parameters::Get( )->Initialise( mRawTextData );

    if( success == true )
        success = ReadTextFile( Constants::cConfigurationDirectory + Constants::cOutputParametersFileName );

    if( success == true )
        success == DataRecorder::Get( )->Initialise( mRawTextData );

    return success;
}

bool FileReader::ReadTextFile( const std::string& filePath ) {
    Logger::Get( )->LogMessage( "Reading text file \"" + filePath + "\"..." );

    ClearRawTextData( );
    std::ifstream fileStream( filePath.c_str( ), std::ios::in );

    if( fileStream.is_open( ) ) {
        std::string readLine;
        unsigned lineCount = 0;

        while( std::getline( fileStream, readLine ) ) {
            if( lineCount > 0 && readLine[ 0 ] != Constants::cTextFileCommentCharacter ) {
                mRawTextData.push_back( Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue ) );
            }
            ++lineCount;
        }
        fileStream.close( );
    } else {
        Logger::Get( )->LogMessage( "File path \"" + filePath + "\" is invalid." );
    }
    DataRecorder::Get( )->AddInputFilePath( filePath );

    return mRawTextData.size( ) > 0;
}

void FileReader::ClearRawTextData( ) {
    for( unsigned rowIndex = 0; rowIndex < mRawTextData.size( ); ++rowIndex ) {
        mRawTextData[ rowIndex ].clear( );
    }
    mRawTextData.clear( );
}
