#include "HeritableTraits.h"
#include "RandomSimple.h"
#include "Parameters.h"
#include "Strings.h"

HeritableTraits::HeritableTraits( const Types::DoubleVector values ) {
    mValues = values;
}

HeritableTraits::HeritableTraits( const Types::DoubleVector values, const Types::BoolVector areTraitsMutations ) {
    mValues = values;
    mAreTraitsMutations = areTraitsMutations;
}

HeritableTraits::~HeritableTraits( ) {
    mValues.clear( );
}

Types::HeritableTraitsPointer HeritableTraits::GetChildTraits( ) {
    Types::BoolVector areTraitsMutations;

    unsigned numberOfGenes = mValues.size( );
    areTraitsMutations.resize( numberOfGenes, false );

    Types::DoubleVector childValues = mValues;

    double mutationProbability = Parameters::Get( )->GetMutationProbability( );

    if( mutationProbability > 0 ) {
        for( unsigned i = 0; i < numberOfGenes; ++i ) {

            if( RandomSimple::Get( )->GetUniform( ) <= mutationProbability ) {
                areTraitsMutations[ i ] = true;

                double mutationValue = RandomSimple::Get( )->GetNormal( 0.0, Parameters::Get( )->GetMutationStandardDeviation( ) );

                childValues[ i ] += mutationValue;

                // Perform reflection on mutations outside the required range 0 to 1.
                if( childValues[ i ] < 0 ) {
                    childValues[ i ] = 0 - childValues[ i ];
                } else if( childValues[ i ] > 1 ) {
                    childValues[ i ] = 2 - childValues[ i ];
                }
            }
        }
    }

    Types::HeritableTraitsPointer childTraits = new HeritableTraits( childValues, areTraitsMutations );

    return childTraits;
}

const Types::BoolVector HeritableTraits::IsMutant( ) const {
    return mAreTraitsMutations;
}

Types::DoubleVector HeritableTraits::GetValues( ) const {
    return mValues;
}

bool HeritableTraits::IsValueMutant( const unsigned geneIndex ) const {
    return mAreTraitsMutations[ geneIndex ];
}

double HeritableTraits::GetValue( Constants::eHeritableTraitIndices index ) const {
    return mValues[ index ];
}

void HeritableTraits::SetValue( Constants::eHeritableTraitIndices index, double geneValue ) {
    mValues[ index ] = geneValue;
}
