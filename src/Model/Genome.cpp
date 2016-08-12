#include "Genome.h"

#include "RandomInterface.h"
#include "Parameters.h"
#include "Convertor.h"
#include "Logger.h"

// For restart

Genome::Genome( const Types::DoubleVector genomeValues ) {
    mGenomeValues = genomeValues;
}

Genome::Genome( const Types::DoubleVector genomeValues, const Types::BoolVector isMutantGenome ) {
    mGenomeValues = genomeValues;
    mIsMutantGenome = isMutantGenome;
}

Genome::~Genome( ) {
    mGenomeValues.clear( );
}

Types::GenomePointer Genome::GetChildGenome( ) {
    Types::BoolVector isMutantChildGenome;

    unsigned numberOfGenes = mGenomeValues.size( );

    isMutantChildGenome.resize( numberOfGenes, false );

    Types::DoubleVector childGenomeValues = mGenomeValues;

    double mutationProbability = Parameters::Get( )->GetMutationProbability( );

    if( mutationProbability > 0 ) {
        for( unsigned geneIndex = 0; geneIndex < numberOfGenes; ++geneIndex ) {

            if( RandomInterface::Get( )->GetUniformDouble( ) <= mutationProbability ) {
                isMutantChildGenome[ geneIndex ] = true;

                double mutationValue = RandomInterface::Get( )->GetNormal( 0, Parameters::Get( )->GetMutationStandardDeviation( ) );

                childGenomeValues[ geneIndex ] += mutationValue;

                // Perform reflection on mutations outside the required range 0 to 1.
                if( childGenomeValues[ geneIndex ] < 0 ) {
                    childGenomeValues[ geneIndex ] = 0 - childGenomeValues[ geneIndex ];
                } else if( childGenomeValues[ geneIndex ] > 1 ) {
                    childGenomeValues[ geneIndex ] = 2 - childGenomeValues[ geneIndex ];
                }
            }
        }
    }

    Types::GenomePointer childGenome = new Genome( childGenomeValues, isMutantChildGenome );

    return childGenome;
}

const Types::BoolVector Genome::IsMutantGenome( ) const {
    return mIsMutantGenome;
}

Types::DoubleVector Genome::GetGenomeValues( ) const {
    return mGenomeValues;
}

bool Genome::IsMutantGeneValue( const unsigned geneIndex ) const {
    return mIsMutantGenome[ geneIndex ];
}

double Genome::GetGeneValue( Constants::eGenomeIndices index ) const {
    return mGenomeValues[ index ];
}

void Genome::SetGeneValue( Constants::eGenomeIndices index, double geneValue ) {
    mGenomeValues[ index ] = geneValue;
}
