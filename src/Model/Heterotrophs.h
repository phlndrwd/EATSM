#ifndef HETEROTROPHS
#define HETEROTROPHS

#include "Heterotrophs.h"
#include "Types.h"

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

    void Feeding( );
    void Metabolisation( );
    void Reproduction( );
    void Starvation( );
    
    void UpdateSizeClasses( );

    void CalculateFeedingProbabilities( );
    void FeedFromAutotrophs( const Types::IndividualPointer );
    void FeedFromHeterotrophs( const Types::IndividualPointer, unsigned );

    void AddToSizeClass( const Types::IndividualPointer );
    void RemoveFromSizeClass( const Types::IndividualPointer );
    void Delete( const Types::IndividualPointer );

    void KillIndividual( const Types::IndividualPointer );
    void DeleteDead( );

    void StarveToDeath( const Types::IndividualPointer );

    unsigned GetSizeClassDeadFrequency( const unsigned ) const;
    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned, const Types::IndividualPointer individual = NULL ) const;

    void AddChild( const Types::IndividualPointer );
    void AddChildren( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;

    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mAutotrophs;

    Types::IndividualMatrix mLivingIndividuals;
    Types::IndividualMatrix mDeadIndividuals;

    Types::IndividualVector mChildren;
};

#endif

