#ifndef CONVERTOR
#define CONVERTOR

#include "Types.h"
#include "Constants.h"

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

class Strings {
public:
    ~Strings( );
    static Types::StringsPointer Get( );

    template< class T >
    const std::string ToString( const T& input ) const {
        std::stringstream stringStream;
        stringStream << input;

        return stringStream.str( );
    }

    double StringToNumber( const std::string& ) const;

    const Types::StringVector StringToWords( const std::string&, const char ) const;
    const std::string DoubleToPrecisionString( const double&, const unsigned& ) const;
    
    int FindFirstPositionOfCharacter( const std::string, const char ) const;
    std::string TruncateStringAtCharacter( const std::string, const char ) const;

    std::string ToLowercase( const std::string ) const;
    std::string RemoveCharacter( const std::string, const char ) const;
    std::string RemoveWhiteSpace( const std::string ) const;

private:
    Strings( );

    static Types::StringsPointer mThis;
};

#endif

