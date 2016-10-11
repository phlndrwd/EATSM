#ifndef HETEROTROPHS
#define	HETEROTROPHS

#include "Heterotrophs.h"
#include "Types.h"

class Heterotrophs {
public:

    Heterotrophs( Types::NutrientPointer, Types::AutotrophsPointer, Types::StringMatrix& );
    ~Heterotrophs( );

    void Update( );
    bool RecordData( );

    unsigned GetSizeClassPopulation( const unsigned ) const;
    Types::IndividualPointer GetIndividual( const unsigned, const unsigned ) const;

private:
    void InitialiseSizeClasses( Types::StringMatrix& );

    void Feeding( );
    void Metabolisation( );
    void Starvation( );
    void Reproduction( );

    void CalculateFeedingProbabilities( );
    void FeedFromPhytoplankton( const Types::IndividualPointer );
    void FeedFromHeterotrophs( const Types::IndividualPointer, unsigned );

    void AddToSizeClass( const Types::IndividualPointer, const bool setSizeClassIndex = true );
    void RemoveFromSizeClass( const Types::IndividualPointer );
    void Delete( const Types::IndividualPointer );

    void KillIndividual( const Types::IndividualPointer );
    void DeleteDead( );

    void StarveToDeath( const Types::IndividualPointer );

    void MoveSizeClass( const Types::IndividualPointer );

    unsigned GetSizeClassDeadFrequency( const unsigned ) const;
    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned, const Types::IndividualPointer individual = NULL ) const;

    void AddChild( const Types::IndividualPointer );
    void AddChildren( );

    Types::HeterotrophProcessorPointer mHeterotrophProcessor;
    Types::HeterotrophDataPointer mHeterotrophData;

    Types::NutrientPointer mNutrient;
    Types::AutotrophsPointer mPhytoplankton;

    Types::IndividualMatrix mSizeClasses;
    Types::IndividualMatrix mDeadFrequencies;

    Types::IndividualArray mChildren;
};

#endif

