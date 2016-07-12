#ifndef GENOME
#define	GENOME

#include "Types.h"
#include "Constants.h"

class Genome {
public:
    Genome( const Types::DoubleVector );
    Genome( const Types::DoubleVector, const Types::BoolVector );
    ~Genome( );

    Types::GenomePointer GetChildGenome( );
    const Types::BoolVector IsMutantGenome( ) const;
    bool IsMutantGeneValue( const unsigned ) const;

    Types::DoubleVector GetGenomeValues( ) const;
    
    double GetGeneValue( Constants::eGenomeIndices ) const;
    void SetGeneValue( Constants::eGenomeIndices, double );

private:
    Types::DoubleVector mGenomeValues;
    Types::BoolVector mIsMutantGenome;
};

#endif

