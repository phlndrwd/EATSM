#include "Strings.h"

#include <algorithm>
#include "Constants.h"

Types::StringsPointer Strings::mThis = NULL;

Types::StringsPointer Strings::Get( ) {
    if( mThis == NULL ) {
        mThis = new Strings( );
    }
    return mThis;
}

Strings::~Strings( ) {
    if( mThis != NULL ) {
        delete mThis;
    }
}

Strings::Strings( ) {

}

double Strings::StringToNumber( const std::string& string ) const {

    double number = strtod( string.c_str( ), NULL );

    return number;
}

const Types::StringVector Strings::StringToWords( const std::string& inputString, const char wordTerminationCharacter ) const {

    std::stringstream stringStream( inputString );

    std::string word = "";
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }

    return wordList;
}

const std::string Strings::DoubleToPrecisionString( const double& value, const unsigned& decimals ) const {

    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}

int Strings::FindFirstPositionOfCharacter( const std::string inString, const char character ) const {
    int index = Constants::cMissingValue;

    for( unsigned charIndex = 0; charIndex < inString.length( ); charIndex++ ) {
        if( inString[ charIndex ] == character ) {
            index = charIndex;
            break;
        }
    }
    return index;
}

std::string Strings::TruncateStringAtCharacter( const std::string inString, const char character ) const {
    std::string outString = inString;
    
    int index = FindFirstPositionOfCharacter( outString, character );
    if( index != Constants::cMissingValue )
        outString = outString.substr( 0, index );
    
    return outString;
}

std::string Strings::ToLowercase( const std::string inString ) const {
    std::string outString;

    std::transform( inString.begin( ), inString.end( ), std::back_inserter( outString ), tolower );

    return outString;
}

std::string Strings::RemoveCharacter( const std::string inString, const char character ) const {
    std::string outString = inString;
    outString.erase( remove( outString.begin( ), outString.end( ), character ), outString.end( ) );

    return outString;
}

std::string Strings::RemoveWhiteSpace( const std::string inString ) const {
    return RemoveCharacter( inString, Constants::cWhiteSpaceCharacter );
}