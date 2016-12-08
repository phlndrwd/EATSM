#ifndef CONVERTOR
#define	CONVERTOR

#include "Types.h"
#include "Constants.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <emmintrin.h> 

class Convertor {
public:
    ~Convertor( );
    static Types::ConvertorPointer Get( );

    template< class T >
    const std::string ToString( const T& input ) const {
        std::stringstream stringStream;
        stringStream << input;

        return stringStream.str( );
    }

    template< typename T >
    std::string M128iToString( __m128i value ) {
        std::stringstream stringStream;
        const T* values = ( const T* ) &value;
        if( sizeof( T ) == 1 ) {
            for( unsigned int index = 0; index < sizeof( __m128i ) - 1; ++index ) {
                stringStream << ( int ) values[ index ] << Constants::cDataDelimiterValue;
            }
            stringStream << ( int ) values[ sizeof( __m128i ) - 1 ];
        } else {
            for( unsigned int index = 0; index < ( sizeof( __m128i ) / sizeof( T ) ) - 1; ++index ) {
                stringStream << values[ index ] << Constants::cDataDelimiterValue;
            }
            stringStream << values[ ( sizeof( __m128i ) / sizeof( T ) ) - 1 ];
        }
        return stringStream.str( );
    }

    double StringToNumber( const std::string& ) const;

    const Types::StringVector StringToWords( const std::string&, const char ) const;
    const std::string DoubleToPrecisionString( const double&, const unsigned& ) const;

    std::string ToLowercase( const std::string ) const;
    std::string RemoveWhiteSpace( const std::string ) const;

    const double GeneValueToVolume( double ) const;
    const double VolumeToGeneValue( double ) const;

private:
    Convertor( );

    static Types::ConvertorPointer mThis;
};

#endif

