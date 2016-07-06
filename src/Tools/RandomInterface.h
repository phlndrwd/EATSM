#ifndef RANDOMINTERFACE
#define	RANDOMINTERFACE

#include "RandomDefault.h"
#include "Types.h"

class RandomInterface {
public:
    ~RandomInterface( );

    static Types::RandomInterfacePointer Get( );

    int GetUniformInt( const float mininimum = Constants::cUniformIntMin, const float maximum = Constants::cUniformIntMax );
    double GetUniformDouble( const float minimum = Constants::cUniformDoubleMin, const float maximum = Constants::cUniformDoubleMax );
    double GetNormal( const float mean = Constants::cNormalMean, const float standardDeviation = Constants::cNormalStdDev );
    
    unsigned GetRandomSeed( ) const;

private:
    RandomInterface( unsigned );

    static Types::RandomInterfacePointer mThis;

    Types::RandomDefaultPointer mRandomDefault;
};

#endif

