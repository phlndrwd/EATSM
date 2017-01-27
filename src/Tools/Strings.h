#ifndef STRINGS
#define	STRINGS

#include "Types.h"
#include "Constants.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <emmintrin.h>
#include <algorithm>


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

    Types::StringVector StringToWords( const std::string&, const char ) const;
    std::string DoubleToPrecisionString( const double&, const unsigned& ) const;

    std::string ToLowercase( const std::string ) const;
    std::string RemoveWhiteSpace( const std::string ) const;

private:
    Strings( );

    static Types::StringsPointer mThis;
};

#endif

