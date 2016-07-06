#include "Convertor.h"
#include "Parameters.h"
#include "Maths.h"

Types::ConvertorPointer Convertor::mThis = 0;

Types::ConvertorPointer Convertor::Get( ) {
    if( mThis == 0 ) {
        mThis = new Convertor( );
    }
    return mThis;
}

Convertor::~Convertor( ) {
    if( mThis != 0 ) {
        delete mThis;
    }
}

Convertor::Convertor( ) {

}

const std::string Convertor::NumberToString( const double number ) const {
    Types::StringStream stringStream;
    stringStream << number;

    return stringStream.str( );
}

double Convertor::StringToNumber( std::string string ) const {

    double number = strtod( string.c_str( ), NULL );

    return number;
}

double Convertor::GeneValueToVolume( double geneValue ) const {

    double volumeExponent = geneValue * ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) + Parameters::Get( )->GetSmallestVolumeExponent( );

    double volume = Maths::Get( )->ToThePower( 10, volumeExponent );

    return volume;
}

double Convertor::VolumeToGeneValue( double volume ) const {

    double geneValue = ( Maths::Get( )->Log10( volume ) - Parameters::Get( )->GetSmallestVolumeExponent( ) ) / ( Parameters::Get( )->GetLargestVolumeExponent( ) - Parameters::Get( )->GetSmallestVolumeExponent( ) );

    return geneValue;
}

const Types::StringVector Convertor::StringToWords( const std::string inputString, const char wordTerminationCharacter ) {

    Types::StringStream stringStream( inputString );

    std::string word;
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }

    return wordList;
}

const std::string Convertor::ParameterNamesListToValuesString( const Types::StringVector parameterNamesList ) {

    Types::StringStream stringStream;

    unsigned int numberOfParameters = parameterNamesList.size( );

    for( unsigned int parameterListIndex = 0; parameterListIndex < numberOfParameters; ++parameterListIndex ) {

        double parameterValue = ParameterHandleToValue( parameterNamesList[ parameterListIndex ] );

        if( parameterValue >= 1e+04 && parameterValue < 1e+05 ) {
            stringStream << "000" << DoubleToPrecisionString( parameterValue, 0 );
        } else if( parameterValue >= 1e+05 && parameterValue < 1e+06 ) {
            stringStream << "00" << DoubleToPrecisionString( parameterValue, 0 );
        } else if( parameterValue >= 1e+06 && parameterValue < 1e+07 ) {
            stringStream << "0" << DoubleToPrecisionString( parameterValue, 0 );
        } else {
            stringStream << DoubleToPrecisionString( parameterValue, 0 );
        }

        if( parameterListIndex < numberOfParameters - 1 ) {
            stringStream << Constants::cStringSplitCharacter;
        }
    }

    return stringStream.str( );
}

double Convertor::ParameterHandleToValue( const std::string parameterHandle ) {

    double value = -1;

    for( unsigned int parameterIndex = 0; parameterIndex < Constants::cNumberOfParameters; ++parameterIndex ) {
        if( parameterHandle == Constants::cParameterHandles[ parameterIndex ] ) {
            value = Parameters::Get( )->GetParameterValueFromIndex( parameterIndex );

            if( parameterIndex == Constants::eFractionalMetabolicExpensePerTimeStep && value != 0 ) {

                value = 0 - Maths::Get( )->Log10( value );

                int roundedValue = Maths::Get( )->Round( value );

                if( roundedValue >= 10 ) {
                    value = ( double )roundedValue;
                }
            }

            break;
        }
    }

    return value;
}

const std::string Convertor::DoubleToPrecisionString( double value, unsigned int decimals ) {

    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}
