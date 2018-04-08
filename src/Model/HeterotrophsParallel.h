#ifndef HETEROTROPHSPARALLEL
#define HETEROTROPHSPARALLEL

#include "Types.h"

class HeterotrophsParallel {
public:
    HeterotrophsParallel( );
    HeterotrophsParallel( Types::NutrientPointer, Types::AutotrophsPointer );
    ~HeterotrophsParallel( );

    void Update( );
    bool RecordData( );

    unsigned GetSizeClassPopulation( const unsigned ) const;
    Types::IndividualPointer GetIndividual( const unsigned, const unsigned ) const;

private:
    void CreateInitialPopulation( );
    void ResetIndividualMatrix( );
    void CalculateFeedingProbabilities( );

    void Feeding( );
    void Metabolisation( );
    void Reproduction( );
    void Starvation( );

    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned, const Types::IndividualPointer individual = NULL ) const;
    void FeedFromAutotrophs( const Types::IndividualPointer );
    void FeedFromHeterotrophs( const Types::IndividualPointer, unsigned );
    void DeleteDead( );
    void DeleteIndividual( Types::IndividualPointer );
    void StarveToDeath( Types::IndividualPointer );
    void KillIndividual( Types::IndividualPointer );
    void AddChildren( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;

    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mAutotrophs;

    Types::IndividualVector mLivingVector;
    Types::IndividualMatrix mLivingMatrix;
    Types::IndividualVector mDeadVector;
    Types::IndividualVector mChildVector;
    Types::UnsignedVector mSizeClassDeadFrequencies;
};

#endif

