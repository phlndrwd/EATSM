#ifndef HETEROTROPHS
#define HETEROTROPHS

#include "HeterotrophProcessor.h"
#include "HeterotrophData.h"
#include "Timer.h"

class Heterotrophs {
public:
    Heterotrophs( Nutrient&, Autotrophs& );
    ~Heterotrophs( );

    void Update( );
    bool RecordData( );

    Types::IndividualPointer GetIndividual( const unsigned, const unsigned );
    std::size_t GetSizeClassPopulation( const unsigned ) const;

private:
    void CreateInitialPopulation( );
    void CalculateFeedingProbabilities( );

    void Feeding( );
    void Metabolisation( );
    void Starvation( );
    void Reproduction( );

    Types::IndividualPointer GetRandomIndividualFromSizeClass( const unsigned );
    Types::IndividualPointer GetRandomPredatorFromSizeClass( const unsigned );
    Types::IndividualPointer GetRandomPreyFromSizeClass( const unsigned, const Types::IndividualPointer );

    void FeedFromAutotrophs( Types::IndividualPointer );
    void FeedFromHeterotrophs( Types::IndividualPointer, const unsigned );

    void StageForMoving( Types::IndividualPointer, const unsigned );
    void MoveSizeClass( Types::IndividualPointer, const unsigned );
    bool RemoveFromSizeClass( Types::IndividualPointer, const unsigned );
    void DeleteIndividual( Types::IndividualPointer );

    void Starve( Types::IndividualPointer );
    void Kill( Types::IndividualPointer );

    void AddChildren( );
    void MoveIndividuals( );
    void DeleteDead( );
    
    Nutrient& mNutrient;
    Autotrophs& mAutotrophs;

    HeterotrophProcessor mHeterotrophProcessor;
    HeterotrophData mHeterotrophData;
    
    Timer mTimer;
    
    const Types::DoubleMatrix mInterSizeClassPreferenceMatrix;
    const Types::DoubleMatrix mInterSizeClassVolumeMatrix;
    
    const Types::DoubleVector mSizeClassMidPoints;
    const Types::UnsignedVector mMaximumSizeClassPopulations;

    const double mSmallestIndividualVolume;
    const double mInitialHeterotrophicVolume;
    const double mSizeClassSubsetFraction;

    const unsigned mNumberOfSizeClasses;
    const unsigned mPreferredPreyVolumeRatio;
    const unsigned mAutotrophSizeClassIndex;

    Types::UnsignedVector mFedCount;
    
    Types::IndividualVector mChildren;
    Types::IndividualIndexVector mIndividualsToMove;

    Types::IndividualMatrix mIndividuals;
    Types::IndividualMatrix mIndividualsDead;
};

#endif

