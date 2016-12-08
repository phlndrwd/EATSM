#include "StringManip.h"

#include "Constants.h"
#include "Parameters.h"

#include <algorithm>
#include <string>

Types::StringManipPointer StringManip::mThis = NULL;

Types::StringManipPointer StringManip::Get( ) {
    if( mThis == NULL ) mThis = new StringManip( );

    return mThis;
}

StringManip::~StringManip( ) {
    if( mThis != NULL ) delete mThis;
}

StringManip::StringManip( ) {

}

double StringManip::StringToNumber( const std::string& inString ) const {
    double number = std::stod( inString.c_str( ), NULL );

    return number;
}

Types::StringVector StringManip::StringToWords( const std::string& inString, const char wordTerminationCharacter ) const {
    std::stringstream stringStream( inString );

    std::string word = "";
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }
    return wordList;
}

std::string StringManip::DoubleToPrecisionString( const double& value, const unsigned& decimals ) const {
    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}

std::string StringManip::ToLowercase( const std::string inString ) const {
    std::string outString;

    std::transform( inString.begin( ), inString.end( ), std::back_inserter( outString ), tolower );

    return outString;
}

std::string StringManip::RemoveWhiteSpace( const std::string inString ) const {
    std::string outString = inString;

    outString.erase( remove( outString.begin( ), outString.end( ), Constants::cWhiteSpaceCharacter ), outString.end( ) );

    return outString;
}
