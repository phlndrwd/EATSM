#include "FileReader.h"

#include "Parameters.h"
#include "Individual.h"
#include "HeritableTraits.h"
#include "Strings.h"
#include "DataRecorder.h"
#include "InitialState.h"

FileReader::FileReader( ) {

}

FileReader::~FileReader( ) {
}

void FileReader::ReadInputFiles( std::string& parametersFile, std::string& stateFile ) {
    bool success = false;

    if( parametersFile == "" ) parametersFile = Constants::cConfigurationDirectory + Constants::cInputParametersFileName;
    if( ReadTextFile( parametersFile ) )
        if( Parameters::Get( )->Initialise( mRawTextData ) )
            if( ReadTextFile( Constants::cConfigurationDirectory + Constants::cOutputParametersFileName ) )
                success = DataRecorder::Get( )->Initialise( mRawTextData );

    if( success == true && Parameters::Get( )->GetReadModelState( ) == true ) {
        success = false;
        if( stateFile == "" ) stateFile = Constants::cConfigurationDirectory + Constants::cInitialStateFileName;
        if( ReadTextFile( stateFile, false ) )
            success = InitialState::Get( )->Initialise( mRawTextData );
    } else if( success == true && Parameters::Get( )->GetReadModelState( ) == false && stateFile != "" ) {
        std::cout << "ERROR> State file \"" << stateFile << "\" specified, but state reading is switched off. System exiting..." << std::endl;
        exit( 1 );
    }

    if( success ) {
        std::cout << "Files read successfully..." << std::endl;
    } else {
        std::cout << "ERROR> File reading failed. System exiting..." << std::endl;
        exit( 1 );
    }
}

bool FileReader::ReadTextFile( const std::string& filePath, bool copyToOutput ) {
    std::cout << "Reading text file \"" << filePath << "\"..." << std::endl;
    ClearRawTextData( );
    std::ifstream fileStream( filePath.c_str( ), std::ios::in );

    if( fileStream.is_open( ) ) {
        std::string readLine;
        unsigned lineCount = 0;

        while( std::getline( fileStream, readLine ) ) {
            if( readLine.length( ) > 0 ) {
                if( readLine[ 0 ] != Constants::cCommentCharacter ) {
                    readLine = Strings::RemoveWhiteSpace( Strings::TruncateStringAtCharacter( readLine, Constants::cCommentCharacter ) );
                    if( lineCount > 0 ) {
                        mRawTextData.push_back( Strings::StringToWords( readLine, Constants::cDataDelimiterValue ) );
                    }
                    lineCount++;
                }
            }
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
