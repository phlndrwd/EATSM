#ifndef HERITABLETRAITS
#define	HERITABLETRAITS

#include "Types.h"
#include "Constants.h"

class HeritableTraits {
public:
    HeritableTraits( const Types::DoubleVector );
    HeritableTraits( const Types::DoubleVector, const Types::BoolVector );
    ~HeritableTraits( );

    Types::HeritableTraitsPointer GetChildTraits( );
    const Types::BoolVector IsMutant( ) const;
    bool IsValueMutant( const unsigned ) const;

    Types::DoubleVector GetValues( ) const;
    
    double GetValue( Constants::eHeritableTraitIndices ) const;
    void SetValue( Constants::eHeritableTraitIndices, double );

private:
    Types::DoubleVector mValues;
    Types::BoolVector mAreTraitsMutations;
};

#endif

