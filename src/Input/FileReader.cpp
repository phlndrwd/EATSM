#include "FileReader.h"

#include "Parameters.h"
#include "Individual.h"
#include "Logger.h"
#include "Genome.h"
#include "Convertor.h"
#include "DataRecorder.h"
#include "InitialState.h"

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

    if( Parameters::Get( )->GetInitialisationMethod( ) == false ) {
        if( success == true )
            success = ReadTextFile( Constants::cConfigurationDirectory + Constants::cInitialisationFileName, false );

        if( success == true )
            success = InitialState::Get( )->Initialise( mRawTextData );
    }

    ClearRawTextData( );

    return success;
}

Types::StringMatrix& FileReader::GetRawTextData( ) {
    return mRawTextData;
}

bool FileReader::ReadTextFile( const std::string& filePath, bool usesHeader ) {
    Logger::Get( )->LogMessage( "Reading text file \"" + filePath + "\"..." );

    ClearRawTextData( );
    std::ifstream fileStream( filePath.c_str( ), std::ios::in );

    if( fileStream.is_open( ) ) {
        std::string readLine;
        unsigned lineCount = 0;

        while( std::getline( fileStream, readLine ) ) {
            if( readLine[ 0 ] != Constants::cTextFileCommentCharacter ) {
                if( usesHeader == true ) {
                    if( lineCount > 0 ) {
                        mRawTextData.push_back( Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue ) );
                    }
                } else {
                    mRawTextData.push_back( Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue ) );
                }
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
