#ifndef HETEROTROPHDATA
#define	HETEROTROPHDATA

#include "Types.h"

class HeterotrophData {
public:
    HeterotrophData( );
    ~HeterotrophData( );

    void RecordOutputData( );

    void InitialiseDataStructures( );
    void ResetDataStructures( );
    
    double GetEffectiveSizeClassVolume( const unsigned, const unsigned ) const;
    void SetEffectiveSizeClassVolume( const unsigned, const unsigned, const double );
    
    unsigned GetCoupledSizeClassIndex( const unsigned );
    double GetEffectivePreyVolume( const unsigned );
    double GetFeedingProbability( const unsigned );

    void SetCoupledSizeClassIndex( const unsigned, const unsigned );
    void SetEffectivePreyVolume( const unsigned, const double );
    void SetFeedingProbability( const unsigned, const double );

    void AddIndividualData( const Types::IndividualPointer );
    void AddSizeClassData( const unsigned, const unsigned );
    void NormaliseData( );

    bool AreHeterotrophsAlive( ) const;

    void IncrementVegetarianFrequencies( const Types::IndividualPointer );
    void IncrementCarnivoreFrequencies( const Types::IndividualPointer, const Types::IndividualPointer );
    void IncrementStarvedFrequencies( const unsigned );
    void IncrementBirthFrequencies( const unsigned, const unsigned );
    void IncrementMutantFrequency( const unsigned, const unsigned );
    
    void AddToTimeFeeding( double );
    void AddToTimeMetabolising( double );
    void AddToTimeReproducing( double );
    void AddToTimeStarving( double );

    unsigned GetVegetarianFrequency( ) const;
    unsigned GetCarnivoreFrequency( ) const;
    unsigned GetChildFrequency( ) const;
    unsigned GetStarvedFrequency( ) const;
    unsigned GetFailedCarnivoreFrequency( ) const;
    unsigned GeFailedVegetarianFrequency( ) const;
    unsigned GetFailedFeedingAttemptFrequency( ) const;

private:
    void AddTrophicLevel( const double, const double, const unsigned, const unsigned );

    Types::FloatMatrix mEffectiveSizeClassVolumeMatrix;
    
    Types::FloatVector mSizeClassPopulation;
    Types::FloatVector mSizeClassHerbivoreFrequencies;
    Types::FloatVector mSizeClassCarnivoreFrequencies;
    Types::FloatVector mSizeClassPreyFrequencies;
    Types::FloatVector mSizeClassStarvedFrequencies;
    Types::FloatVector mSizeClassParentFrequencies;
    Types::FloatVector mSizeClassChildFrequencies;
    Types::FloatVector mSizeClassVolumeMutantFrequencies;
    Types::FloatVector mSizeClassVolumes;
    Types::FloatVector mSizeClassApproxVolumes;
    Types::FloatVector mSizeClassEffectivePreyVolumes;
    Types::FloatVector mSizeClassGrowthRatios;
    Types::FloatVector mSizeClassCouplings;
    Types::FloatVector mSizeClassPreyVolumeRatios;
    Types::FloatVector mSizeClassFeedingProbabilities;
    Types::FloatVector mSizeClassTrophicClassifications;
    Types::FloatVector mSizeClassAges;

    Types::FloatVector mTrophicFrequencies;
    Types::FloatVector mTrophicVolumes;
    Types::FloatVector mTrophicAges;
    
    unsigned mFrequency;
    double mVolume;
    double mApproxVolume;
    double mToFlux;
    double mInFlux;
    
    double mTimeFeeding;
    double mTimeMetabolising;
    double mTimeReproducing;
    double mTimeStarving;
};

#endif
