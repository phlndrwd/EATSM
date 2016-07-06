#include "Random.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>

Random::Random( const unsigned int seed ) {
    srand( seed );
}

double Random::UniformDouble( ) {
    return ( double )rand( ) / RAND_MAX;
}

double Random::UniformScaledDouble( double minimum, double maximum ) {
    return ( UniformDouble( ) * ( maximum - minimum ) ) + minimum;
}

// Polar form of the Box-Muller transformation.
// Mean and standard deviation default to 0.0 and 1.0, see header.
// Algorithm optimization achieved by returning result from one uniform random
// and caching result from second for use on the second call.
// ftp://ftp.taygeta.com/pub/c/boxmuller.c

double Random::NormalDouble( const double mean, const double standardDeviation ) {

    double randomUniformOne;
    double randomUniformTwo;
    double randomSumSquare;
    double randomNormalOne;
    static double randomNormalTwo;
    static bool mIsCalculated = 0;


    if( mIsCalculated ) { // Use value from previous call
        randomNormalOne = randomNormalTwo;
        mIsCalculated = false;
    } else {
        do {
            randomUniformOne = 2.0 * UniformDouble( ) - 1.0;
            randomUniformTwo = 2.0 * UniformDouble( ) - 1.0;
            randomSumSquare = randomUniformOne * randomUniformOne + randomUniformTwo * randomUniformTwo;
        } while( randomSumSquare >= 1.0 );

        randomSumSquare = sqrt( ( -2.0 * log( randomSumSquare ) ) / randomSumSquare );
        randomNormalOne = randomUniformOne * randomSumSquare;
        randomNormalTwo = randomUniformTwo * randomSumSquare;
        mIsCalculated = true;
    }

    return ( mean + randomNormalOne * standardDeviation );
}

// Generates a random number between 0 (inclusive) and max (inclusive).

int Random::UniformInteger( const int maximum ) {
    return rand( ) % ( maximum + 1 );
}
