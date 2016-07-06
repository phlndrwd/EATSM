#ifndef RANDOMDEFAULT
#define	RANDOMDEFAULT

#include <random>
#include "Constants.h"

class RandomDefault {
public:
    RandomDefault( unsigned seed = 0 );

    int GetUniformInt( const float minimum = Constants::cUniformIntMin, const float maximum = Constants::cUniformIntMax );
    double GetUniformDouble( const float minimum = Constants::cUniformDoubleMin, const float maximum = Constants::cUniformDoubleMax );
    double GetNormal( const float mean = Constants::cNormalMean, const float standardDeviation = Constants::cNormalStdDev );
    
    unsigned GetSeed( );

private:
    std::random_device mRandomDevice;
    std::default_random_engine mEngine;
    std::uniform_real_distribution< double > mUniformDoubleDistribution;
    std::uniform_int_distribution< int > mUniformIntDistribution;
    std::normal_distribution< double > mNormalDistribution;
    
    int mUniformIntMin;
    int mUniformIntMax;
    
    float mUniformDoubleMin;
    float mUniformDoubleMax;
    
    float mNormalMean;
    float mNormalStdDev;
    
    unsigned mSeed;
};

#endif

