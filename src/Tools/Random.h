#ifndef RANDOMSTANDARD
#define RANDOMSTANDARD

class Random {
public:
    Random( unsigned int );

    double UniformDouble( );
    double UniformScaledDouble( double, double maximum = 1.0 );
    double NormalDouble( const double mean = 0.0, const double standardDeviation = 1.0 );
    int UniformInteger( const int );
};

#endif
