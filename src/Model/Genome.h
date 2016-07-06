#ifndef GENOME
#define	GENOME

#include "Types.h"
#include "Constants.h"

class Genome {
public:
    Genome( const Types::DoubleVector );
    Genome( const Types::DoubleVector, const Types::BooleanVector );
    ~Genome( );

    Types::GenomePointer GetChildGenome( );
    const Types::BooleanVector IsMutantGenome( ) const;
    bool IsMutantGeneValue( const unsigned int ) const;

    Types::DoubleVector GetGenomeValues( ) const;
    
    double GetGeneValue( Constants::eGenomeIndices ) const;
    void SetGeneValue( Constants::eGenomeIndices, double );

private:
    Types::DoubleVector mGenomeValues;
    Types::BooleanVector mIsMutantGenome;
};

#endif

