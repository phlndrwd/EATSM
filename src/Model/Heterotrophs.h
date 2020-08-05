#ifndef HETEROTROPHS
#define HETEROTROPHS

#include "Types.h"
#include "Timer.h"

class Heterotrophs {
public:
    Heterotrophs( );
    Heterotrophs( Types::NutrientPointer, Types::AutotrophsPointer );
    ~Heterotrophs( );

    void Update( );
    bool RecordData( );
    
    unsigned GetSizeClassPopulation( const unsigned ) const;
    Types::IndividualPointer GetIndividual( const unsigned, const unsigned ) const;

private:
    void CreateInitialPopulation( );
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
    
    void MoveSizeClass( const Types::IndividualPointer, const unsigned );
    
    void StarveToDeath( Types::IndividualPointer );
    void KillIndividual( Types::IndividualPointer );
    void AddChildren( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;

    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mAutotrophs;

    Types::IndividualMatrix mLivingMatrix;
    Types::IndividualMatrix mDeadMatrix;
    Types::IndividualVector mChildVector;
    Timer mTimer;
};

#endif

