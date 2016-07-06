#ifndef MATH
#define	MATH

#include "Types.h"

class Maths {
public:
    ~Maths( );
    static Types::MathPointer Get( );

    int RoundWithProbability( const double ) const;

    int RoundDown( const double ) const;
    int RoundUp( const double ) const;
    int Round( const double ) const;

    double Absolute( const double ) const;
    
    double Min( const double, const double ) const;
    double Max( const double, const double ) const;

    double Log( const double ) const;
    double Log10( const double ) const;
    double Exponential( const double ) const;

    double ToThePower( const double, const double ) const;
    double NthRoot( const double, const double ) const;

private:
    Maths( );

    static Types::MathPointer mThis;
};

#endif

