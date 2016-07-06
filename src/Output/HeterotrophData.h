#ifndef HETEROTROPHDATA
#define	HETEROTROPHDATA

#include "Types.h"

class HeterotrophData {
public:
    HeterotrophData( );
    ~HeterotrophData( );

    void RecordOutputData( );
    void OutputSummary( );

    void InitialiseDataStructures( );
    void ResetDataStructures( );
    
    unsigned int GetProbabilisticPreySizeClassIndex( const unsigned int ) const;
    
    void CalculateSizeClassInteractionProbabilities( );
    double GetEffectiveSizeClassVolume( const unsigned int, const unsigned int ) const;
    void SetEffectiveSizeClassVolume( const unsigned int, const unsigned int, const double );
    
    unsigned int GetCoupledSizeClassIndex( const unsigned int );
    double GetEffectivePreyVolume( const unsigned int );
    double GetFeedingProbability( const unsigned int );

    void SetCoupledSizeClassIndex( const unsigned int, const unsigned int );
    void SetEffectivePreyVolume( const unsigned int, const double );
    void SetFeedingProbability( const unsigned int, const double );

    void AddIndividualData( const Types::IndividualPointer );
    void AddSizeClassData( const unsigned int, const unsigned int );
    void NormaliseData( );

    bool AreHeterotrophsAlive( ) const;

    void IncrementVegetarianFrequencies( const Types::IndividualPointer );
    void IncrementCarnivoreFrequencies( const Types::IndividualPointer, const Types::IndividualPointer );
    void IncrementFailedFeedingAttemptFrequency( );
    void IncrementFailedVegetarianFrequency( );
    void IncrementFailedCarnivoreFrequency( );
    void IncrementStarvedFrequencies( const unsigned int );
    void IncrementBirthFrequencies( const unsigned int, const unsigned int );
    void IncrementMutantFrequency( const unsigned int, const unsigned int );

    unsigned int GetVegetarianFrequency( ) const;
    unsigned int GetCarnivoreFrequency( ) const;
    unsigned int GetChildFrequency( ) const;
    unsigned int GetStarvedFrequency( ) const;
    unsigned int GetFailedCarnivoreFrequency( ) const;
    unsigned int GeFailedVegetarianFrequency( ) const;
    unsigned int GetFailedFeedingAttemptFrequency( ) const;

private:
    void AddTrophicLevel( const double, const double, const unsigned int, const unsigned int );

    Types::FloatMatrix mEffectiveSizeClassVolumeMatrix;
    Types::FloatMatrix mSizeClassInteractionProbabilityMatrix;
    
    Types::IntegerVector mSizeClassSizes;
    Types::IntegerVector mSizeClassVegetarianFrequencies;
    Types::IntegerVector mSizeClassCarnivoreFrequencies;
    Types::IntegerVector mSizeClassPreyFrequencies;
    Types::IntegerVector mSizeClassStarvedFrequencies;
    Types::IntegerVector mSizeClassParentFrequencies;
    Types::IntegerVector mSizeClassChildFrequencies;
    Types::IntegerVector mSizeClassVolumeMutantFrequencies;
    Types::FloatVector mSizeClassVolumes;
    Types::FloatVector mSizeClassApproximateVolumes;
    Types::FloatVector mSizeClassEffectivePreyVolumes;
    Types::FloatVector mSizeClassGrowthRatios;
    Types::IntegerVector mSizeClassCouplings;
    Types::FloatVector mSizeClassPreyVolumeRatios;
    Types::FloatVector mSizeClassFeedingProbabilities;
    Types::FloatVector mSizeClassTrophicClassifications;
    Types::FloatVector mSizeClassAges;

    Types::IntegerVector mTrophicFrequencies;
    Types::FloatVector mTrophicVolumes;
    Types::FloatVector mTrophicAges;
    
    unsigned int mFrequency;
    double mVolume;
    double mApproximateVolume;
    double mToFlux;
    double mInFlux;

    unsigned int mVegetarianFrequency;
    unsigned int mCarnivoreFrequency;
    unsigned int mBirthFrequency;
    unsigned int mStarvedFrequency;
    unsigned int mFailedCarnivoreFrequency;
    unsigned int mFailedVegetarianFrequency;
    unsigned int mFailedFeedingAttemptFrequency;
};

#endif
