#include "FileReader.h"

#include "Parameters.h"
#include "Individual.h"
#include "Genome.h"
#include "StringTools.h"
#include "DataRecorder.h"
#include "InitialState.h"

FileReader::FileReader( ) {

}

FileReader::~FileReader( ) {
}

bool FileReader::ReadInputFiles( std::string& stateFile ) {
    bool success = ReadTextFile( Constants::cConfigurationDirectory + Constants::cInputParametersFileName );

    if( success == true )
        success = Parameters::Get( )->Initialise( mRawTextData );

    if( success == true )
        success = ReadTextFile( Constants::cConfigurationDirectory + Constants::cOutputParametersFileName );

    if( success == true )
        success = DataRecorder::Get( )->Initialise( mRawTextData );

    if( Parameters::Get( )->GetCreateNewPopulation( ) == false ) {
        if( success == true ) {
            if( stateFile == "" ) stateFile = Constants::cConfigurationDirectory + Constants::cInitialStateFileName;
            success = ReadTextFile( stateFile, false );
        }

        if( success == true )
            success = InitialState::Get( )->Initialise( mRawTextData );
    }

    ClearRawTextData( );

    return success;
}

Types::StringMatrix& FileReader::GetRawTextData( ) {
    return mRawTextData;
}

bool FileReader::ReadTextFile( const std::string& filePath, bool copyToOutput ) {
    std::cout << "Reading text file \"" << filePath << "\"..." << std::endl;
    ClearRawTextData( );
    std::ifstream fileStream( filePath.c_str( ), std::ios::in );

    if( fileStream.is_open( ) ) {
        std::string readLine;
        unsigned lineCount = 0;

        while( std::getline( fileStream, readLine ) ) {
            if( readLine[ 0 ] != Constants::cTextFileCommentCharacter && lineCount > 0 ) {
                Types::StringVector stringVec = StringTools::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue );
                mRawTextData.push_back( StringTools::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue ) );
            }
            ++lineCount;
        }
        fileStream.close( );
    } else {
        std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;
    }
    if( copyToOutput == true ) DataRecorder::Get( )->AddInputFilePath( filePath );

    return mRawTextData.size( ) > 0;
}

void FileReader::ClearRawTextData( ) {
    for( unsigned rowIndex = 0; rowIndex < mRawTextData.size( ); ++rowIndex ) {
        mRawTextData[ rowIndex ].clear( );
    }
    mRawTextData.clear( );
}
