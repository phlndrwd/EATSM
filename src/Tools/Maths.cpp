#include "Maths.h"

#include "RandomInterface.h"

#include <math.h>
#include <cmath> 

Types::MathPointer Maths::mThis = 0;

Types::MathPointer Maths::Get( ) {
    if( mThis == 0 ) {
        mThis = new Maths( );
    }
    return mThis;
}

Maths::~Maths( ) {
    if( mThis != 0 ) {
        delete mThis;
    }
}

Maths::Maths( ) {

}

 int Maths::RoundWithProbability( const double value ) const {

    int flooredValue = RoundDown( value );

    double probability = value - flooredValue;

    if( RandomInterface::Get( )->GetUniformDouble( ) < probability ) {
        return flooredValue + 1;
    } else {
        return flooredValue;
    }
}

 int Maths::RoundDown( const double value ) const {

    int roundedValue = static_cast < int >( ::floor( value ) );

    return roundedValue;
}

 int Maths::RoundUp( const double value ) const {

    int roundedValue = static_cast < int >( ::ceil( value ) );

    return roundedValue;
}

 int Maths::Round( const double value ) const {
    
    int roundedValue = static_cast < int >( ::floor( value + 0.5 ) );

    return roundedValue;
}

double Maths::Absolute( const double value ) const {
    return std::abs( value );
}

 double Maths::Min(const double a, const double b ) const {
    return ( a < b ) ? a : b;
}

 double Maths::Max( const double a,const double b ) const {
    return ( a > b ) ? a : b;
}

 double Maths::Log( const double value ) const {
    return log( value );
}

 double Maths::Log10( const double value ) const {
   return log10( value );
}

 double Maths::Exponential( const double exponent ) const {
    return exp( exponent );
}

 double Maths::ToThePower( const double base, const double exponent ) const {
    return pow( base, exponent );
}

 double Maths::NthRoot( const double root, const double degree ) const {
    return pow( root, 1.0 / degree );
}
