#include "RandomDefault.h"

RandomDefault::RandomDefault( unsigned seed ): mUniformIntDistribution( Constants::cUniformIntMin, Constants::cUniformIntMax ), mUniformDoubleDistribution( Constants::cUniformDoubleMin, Constants::cUniformDoubleMax ), mNormalDistribution( Constants::cNormalMean, Constants::cNormalStdDev ) {

    if( seed == 1 )
        mSeed = mRandomDevice( );
    else
        mSeed = seed;

    mEngine.seed( mSeed );

    mUniformIntMin = Constants::cUniformIntMin;
    mUniformIntMax = Constants::cUniformIntMax;

    mUniformDoubleMin = Constants::cUniformDoubleMin;
    mUniformDoubleMax = Constants::cUniformDoubleMax;

    mNormalMean = Constants::cNormalMean;
    mNormalStdDev = Constants::cNormalStdDev;
}

int RandomDefault::GetUniformInt( const float minimum, const float maximum ) {

    if( minimum != mUniformIntMin || maximum != mUniformIntMax ) {
        mUniformIntMin = minimum;
        mUniformIntMax = maximum;

        std::uniform_int_distribution< >::param_type customParams{ mUniformIntMin, mUniformIntMax };
        mUniformIntDistribution.param( customParams );
    }

    return mUniformIntDistribution( mEngine );
}

double RandomDefault::GetUniformDouble( const float minimum, const float maximum ) {

    if( minimum != mUniformDoubleMin || maximum != mUniformDoubleMax ) {
        mUniformDoubleMin = minimum;
        mUniformDoubleMax = maximum;

        std::uniform_real_distribution< >::param_type customParams{ mUniformDoubleMin, mUniformDoubleMax };
        mUniformDoubleDistribution.param( customParams );
    }

    return mUniformDoubleDistribution( mEngine );
}

double RandomDefault::GetNormal( const float mean, const float standardDeviation ) {

    if( mean != mNormalMean || standardDeviation != mNormalStdDev ) {
        mNormalMean = mean;
        mNormalStdDev = standardDeviation;

        std::normal_distribution< >::param_type customParams{ mNormalMean, mNormalStdDev };
        mNormalDistribution.param( customParams );
    }

    return mNormalDistribution( mEngine );
}

unsigned RandomDefault::GetSeed( ) {
    return mSeed;
}

