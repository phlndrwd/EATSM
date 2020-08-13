#ifndef RANDOMSIMPLE
#define	RANDOMSIMPLE

#include "Types.h"

/*
* Written by John D. Cook 
* http://www.johndcook.com
* https://www.codeproject.com/Articles/25172/Simple-Random-Number-Generation
*/
class RandomSimple {
    
public:
    static Types::RandomSimplePointer Get( );
    ~RandomSimple( );
    
    void Reset( );
    
    unsigned GetSeed( ) const;
    void SetSeed( unsigned, unsigned );
    void SetSeed( unsigned );
    
    double GetUniform( );
    unsigned GetUniformInt( );
    unsigned GetUniformInt( unsigned );
    double GetNormal( );
    double GetNormal( double, double );
    double GetExponential( );
    double GetExponential( double );
    double GetGamma( double, double );
    double GetChiSquare( double );
    double GetInverseGamma( double, double );
    double GetWeibull( double, double );
    double GetCauchy( double, double );
    double GetLaplace( double, double );
    double GetLogNormal( double, double );
    double GetBeta( double, double );
    
private:
    RandomSimple( );
    
    static Types::RandomSimplePointer mThis;
    
    unsigned mSeed;
    unsigned mW;
    unsigned mZ;
    double mTwoPi;
};

#endif