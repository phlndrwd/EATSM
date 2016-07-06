#ifndef CONVERTOR
#define	CONVERTOR

#include "Types.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

class Convertor {
public:
    ~Convertor( );
    static Types::ConvertorPointer Get( );

    const std::string NumberToString( double ) const;
    double StringToNumber( std::string ) const;

    double GeneValueToVolume( double ) const;
    double VolumeToGeneValue( double ) const;

    const Types::StringVector StringToWords( const std::string, const char );
    const std::string ParameterNamesListToValuesString( const Types::StringVector );
    const std::string DoubleToPrecisionString( double, unsigned int );

private:
    Convertor( );

    double ParameterHandleToValue( const std::string );

    static Types::ConvertorPointer mThis;
};

#endif

