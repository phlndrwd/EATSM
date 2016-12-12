#include "Strings.h"

#include "Constants.h"
#include "Parameters.h"

Types::StringsPointer Strings::mThis = NULL;

Types::StringsPointer Strings::Get( ) {
    if( mThis == NULL ) mThis = new Strings( );

    return mThis;
}

Strings::~Strings( ) {
    if( mThis != NULL ) delete mThis;
}

Strings::Strings( ) {

}

double Strings::StringToNumber( const std::string& inString ) const {
    //double number = std::stod( inString.c_str( ), NULL );
    double number = strtod( inString.c_str( ), NULL );

    return number;
}

Types::StringVector Strings::StringToWords( const std::string& inString, const char wordTerminationCharacter ) const {
    std::stringstream stringStream( inString );

    std::string word = "";
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }
    return wordList;
}

std::string Strings::DoubleToPrecisionString( const double& value, const unsigned& decimals ) const {
    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}

std::string Strings::ToLowercase( const std::string inString ) const {
    std::string outString;

    std::transform( inString.begin( ), inString.end( ), std::back_inserter( outString ), tolower );

    return outString;
}

std::string Strings::RemoveWhiteSpace( const std::string inString ) const {
    std::string outString = inString;

    outString.erase( remove( outString.begin( ), outString.end( ), Constants::cWhiteSpaceCharacter ), outString.end( ) );

    return outString;
}
