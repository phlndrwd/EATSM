#include "StringTools.h"

#include "Constants.h"
#include "Parameters.h"

#include <algorithm>
#include <string>

Types::StringManipPointer StringTools::mThis = NULL;

Types::StringManipPointer StringTools::Get( ) {
    if( mThis == NULL ) mThis = new StringTools( );

    return mThis;
}

StringTools::~StringTools( ) {
    if( mThis != NULL ) delete mThis;
}

StringTools::StringTools( ) {

}

double StringTools::StringToNumber( const std::string& inString ) const {
    double number = std::stod( inString.c_str( ), NULL );

    return number;
}

Types::StringVector StringTools::StringToWords( const std::string& inString, const char wordTerminationCharacter ) const {
    std::stringstream stringStream( inString );

    std::string word = "";
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }
    return wordList;
}

std::string StringTools::DoubleToPrecisionString( const double& value, const unsigned& decimals ) const {
    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}

std::string StringTools::ToLowercase( const std::string inString ) const {
    std::string outString;

    std::transform( inString.begin( ), inString.end( ), std::back_inserter( outString ), tolower );

    return outString;
}

std::string StringTools::RemoveWhiteSpace( const std::string inString ) const {
    std::string outString = inString;

    outString.erase( remove( outString.begin( ), outString.end( ), Constants::cWhiteSpaceCharacter ), outString.end( ) );

    return outString;
}
