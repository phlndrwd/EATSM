/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef HERITABLETRAITS
#define	HERITABLETRAITS

#include "RandomSimple.h"
#include "Constants.h"

class HeritableTraits {
public:
    HeritableTraits( const Types::DoubleVector&, const Types::BoolVector& );
    ~HeritableTraits( );
    HeritableTraits& operator = ( const HeritableTraits& );

    HeritableTraits GetChildTraits( RandomSimple& );
    const std::vector< double >& GetValues( ) const;
    const std::vector< bool >& AreTraitsMutant( ) const;
    
    const double& GetValue( const Constants::eHeritableTraitIndices ) const;
    bool IsTraitMutant( const unsigned ) const;
    
    void SetValue( const Constants::eHeritableTraitIndices, const double );

private:
    const double mMutationProbability;
    const double mMutationStandardDeviation;
    
    Types::DoubleVector mValues;
    Types::BoolVector mAreMutantTraits;
};

#endif

