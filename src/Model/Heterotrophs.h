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
    
    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned ) const;
    Types::IndividualPointer GetRandomPredatorFromSizeClass( const unsigned ) const ;
    Types::IndividualPointer GetRandomPreyFromSizeClass( const unsigned, const Types::IndividualPointer individual ) const;
    
    void FeedFromAutotrophs( const Types::IndividualPointer );
    void FeedFromHeterotrophs( const Types::IndividualPointer, unsigned );
    
    void StageForMoving( const Types::IndividualPointer, const unsigned );
    void MoveSizeClass( const Types::IndividualPointer, const unsigned );
    void RemoveFromSizeClass( const Types::IndividualPointer, const unsigned );
    void DeleteIndividual( Types::IndividualPointer );
    
    void StarveToDeath( Types::IndividualPointer );
    void KillIndividual( Types::IndividualPointer );
    
    void AddChildren( );
    void MoveIndividuals( );
    void DeleteDead( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;

    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mAutotrophs;

    Types::UnsignedVector mFedCount;
    Types::UnsignedVector mOldSizeClassIndicies;
    Types::IndividualVector mIndividualsToMove;
    
    Types::IndividualMatrix mIndividualsLiving;
    Types::IndividualMatrix mIndividualsDead;
    Types::IndividualVector mChildren;
    Timer mTimer;
};

#endif

