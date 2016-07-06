#ifndef HETEROTROPHS
#define	HETEROTROPHS

#include "Heterotrophs.h"
#include "Types.h"

class Heterotrophs {
public:

    Heterotrophs( Types::NutrientPointer, Types::PhytoplanktonPointer );
    ~Heterotrophs( );

    void Update( );
    bool RecordData( );
    void OutputSummaryData( );
    
    Types::HeterotrophDataPointer GetHeterotrophData( ) const;

    // Public for restart
    unsigned int GetSizeClassSize( const unsigned int ) const;
    Types::IndividualPointer GetIndividual( const unsigned int, const unsigned int );

private:
    void InitialiseSizeClasses( );

    void Feeding( );
    void Metabolisation( );
    void Starvation( );
    void Reproduction( );

    void CalculateFeedingProbabilities( );
    void FeedFromPhytoplankton( const Types::IndividualPointer );
    void FeedFromHeterotrophs( const Types::IndividualPointer, unsigned int );

    void AddToSizeClass( const Types::IndividualPointer, const bool setSizeClassIndex = true );
    void RemoveFromSizeClass( const Types::IndividualPointer );
    void Delete( const Types::IndividualPointer );

    void KillIndividual( const Types::IndividualPointer );
    void DeleteDead( );

    void StarveToDeath( const Types::IndividualPointer );

    void MoveSizeClass( const Types::IndividualPointer );
    
    unsigned int GetSizeClassDeadFrequency( const unsigned int ) const;
    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned int, const Types::IndividualPointer individual = 0 ) const;

    void AddChild( const Types::IndividualPointer );
    void AddChildren( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;
    
    Types::NutrientPointer mNutrient;
    Types::PhytoplanktonPointer mPhytoplankton;

    Types::IndividualMatrix mSizeClasses;
    Types::IndividualMatrix mDeadFrequencies;

    Types::IndividualArray mChildren;
};

#endif

