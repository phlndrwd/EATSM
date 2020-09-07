#ifndef HERITABLETRAITS
#define	HERITABLETRAITS

#include "Constants.h"

class HeritableTraits {
public:
    HeritableTraits( const std::vector< double >&, const std::vector< bool >& );
    ~HeritableTraits( );

    HeritableTraits GetChildTraits( );
    const std::vector< double >& GetValues( ) const;
    const std::vector< bool >& AreTraitsMutant( ) const;
    
    const double& GetValue( const Constants::eHeritableTraitIndices ) const;
    bool IsTraitMutant( const unsigned ) const;
    
    void SetValue( const Constants::eHeritableTraitIndices, const double );

private:
    std::vector< double > mValues;
    std::vector< bool > mAreMutantTraits;
};

#endif

