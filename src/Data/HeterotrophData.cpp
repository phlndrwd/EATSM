#include "HeterotrophData.h"

#include "DataRecorder.h"
#include "Parameters.h"
#include "Strings.h"
#include "Individual.h"
#include "HeritableTraits.h"

HeterotrophData::HeterotrophData( ) {
    // Convert double vectors used in the model to floats for writing to file
    Types::DoubleVector sizeClassMidPoints = Parameters::Get( )->GetSizeClassMidPoints( );
    Types::DoubleVector sizeClassBoundaries = Parameters::Get( )->GetSizeClassBoundaries( );
    
    Types::FloatVector sizeClassMidPointsFloat( sizeClassMidPoints.begin( ), sizeClassMidPoints.end( ) );
    Types::FloatVector sizeClassBoundariesFloat( sizeClassBoundaries.begin( ), sizeClassBoundaries.end( ) );
    
    DataRecorder::Get( )->SetVectorDataOn( "AxisSizeClassMidPointValues", sizeClassMidPointsFloat );
    DataRecorder::Get( )->SetVectorDataOn( "AxisSizeClassBoundaryValues", sizeClassBoundariesFloat );

    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );
    mEffectiveSizeClassVolumeMatrix.resize( numberOfSizeClasses );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses; ++sizeClassIndex ) {
        mEffectiveSizeClassVolumeMatrix[ sizeClassIndex ].resize( numberOfSizeClasses, 0 );
    }

    mSizeClassCouplings.resize( numberOfSizeClasses, Constants::cMissingValue );
    mSizeClassEffectivePreyVolumes.resize( numberOfSizeClasses, 0 );
    mSizeClassFeedingProbabilities.resize( numberOfSizeClasses, 0 );
    mSizeClassHerbivoreFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassCarnivoreFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassPreyFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassPreyVolumeRatios.resize( numberOfSizeClasses, 0 );
    mSizeClassParentFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassChildFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassVolumeMutantFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassStarvedFrequencies.resize( numberOfSizeClasses, 0 );

    mInFlux = 0;
    mToFlux = 0;
    mVolume = 0;
    mApproxVolume = 0;
    mFrequency = 0;
}

HeterotrophData::~HeterotrophData( ) {

}

void HeterotrophData::InitialiseDataStructures( ) {
    mSizeClassPopulation.clear( );
    mSizeClassVolumes.clear( );
    mSizeClassApproxVolumes.clear( );
    mSizeClassGrowthRatios.clear( );
    mSizeClassTrophicClassifications.clear( );
    mSizeClassAges.clear( );

    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );
    mSizeClassPopulation.resize( numberOfSizeClasses, Constants::cMissingValue );
    mSizeClassVolumes.resize( numberOfSizeClasses, 0 );
    mSizeClassApproxVolumes.resize( numberOfSizeClasses, Constants::cMissingValue );
    mSizeClassGrowthRatios.resize( numberOfSizeClasses, 0 );
    mSizeClassTrophicClassifications.resize( numberOfSizeClasses, 0 );
    mSizeClassAges.resize( numberOfSizeClasses, 0 );

    mTrophicFrequencies.clear( );
    mTrophicVolumes.clear( );
    mTrophicAges.clear( );

    mTrophicFrequencies.resize( Constants::cMaximumNumberOfTrophicLevels, 0 ); // 0 = unclassified, 1 = primary producer, 2 = secondary producer
    mTrophicVolumes.resize( Constants::cMaximumNumberOfTrophicLevels, 0 );
    mTrophicAges.resize( Constants::cMaximumNumberOfTrophicLevels, 0 );

    mVolume = 0;
    mApproxVolume = 0;
    mFrequency = 0;
}

void HeterotrophData::RecordOutputData( ) {
    DataRecorder::Get( )->AddDataTo( "HeterotrophFrequency", mFrequency );
    DataRecorder::Get( )->AddDataTo( "HeterotrophVolume", mVolume );
    DataRecorder::Get( )->AddDataTo( "HeterotrophApproxVolume", mApproxVolume );
    DataRecorder::Get( )->AddDataTo( "ToHeterotrophFlux", mToFlux );
    DataRecorder::Get( )->AddDataTo( "InHeterotrophFlux", mInFlux );

    DataRecorder::Get( )->AddDataTo( "TimingFeeding", mTimeFeeding );
    DataRecorder::Get( )->AddDataTo( "TimingMetabolising", mTimeMetabolising );
    DataRecorder::Get( )->AddDataTo( "TimingReproducing", mTimeReproducing );
    DataRecorder::Get( )->AddDataTo( "TimingStarving", mTimeStarving );

    DataRecorder::Get( )->AddDataTo( "SizeClassPopulation", mSizeClassPopulation );
    DataRecorder::Get( )->AddDataTo( "SizeClassHerbivoreFrequencies", mSizeClassHerbivoreFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassCarnivoreFrequencies", mSizeClassCarnivoreFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassPreyFrequencies", mSizeClassPreyFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassStarvedFrequencies", mSizeClassStarvedFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassParentFrequencies", mSizeClassParentFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassChildFrequencies", mSizeClassChildFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassVolumeMutantFrequencies", mSizeClassVolumeMutantFrequencies );
    DataRecorder::Get( )->AddDataTo( "SizeClassVolumes", mSizeClassVolumes );
    DataRecorder::Get( )->AddDataTo( "SizeClassApproxVolumes", mSizeClassApproxVolumes );
    DataRecorder::Get( )->AddDataTo( "SizeClassEffectivePreyVolumes", mSizeClassEffectivePreyVolumes );
    DataRecorder::Get( )->AddDataTo( "SizeClassGrowthRatios", mSizeClassGrowthRatios );
    DataRecorder::Get( )->AddDataTo( "SizeClassCouplings", mSizeClassCouplings );
    DataRecorder::Get( )->AddDataTo( "SizeClassPreyVolumeRatios", mSizeClassPreyVolumeRatios );
    DataRecorder::Get( )->AddDataTo( "SizeClassFeedingProbabilities", mSizeClassFeedingProbabilities );
    DataRecorder::Get( )->AddDataTo( "SizeClassTrophicClassifications", mSizeClassTrophicClassifications );
    DataRecorder::Get( )->AddDataTo( "SizeClassAges", mSizeClassAges );

    DataRecorder::Get( )->AddDataTo( "TrophicFrequencies", mTrophicFrequencies );
    DataRecorder::Get( )->AddDataTo( "TrophicVolumes", mTrophicVolumes );
    DataRecorder::Get( )->AddDataTo( "TrophicAges", mTrophicAges );

    ResetDataStructures( );
}

double HeterotrophData::GetEffectiveSizeClassVolume( const unsigned predatorIndex, const unsigned preyIndex ) const {
    return mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ];
}

void HeterotrophData::SetEffectiveSizeClassVolume( const unsigned predatorIndex, const unsigned preyIndex, const double effectiveSizeClassVolume ) {
    mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] = effectiveSizeClassVolume;
}

unsigned HeterotrophData::GetCoupledSizeClassIndex( const unsigned sizeClassIndex ) {
    return mSizeClassCouplings[ sizeClassIndex ];
}

double HeterotrophData::GetEffectivePreyVolume( const unsigned sizeClassIndex ) {
    return mSizeClassEffectivePreyVolumes[ sizeClassIndex ];
}

double HeterotrophData::GetFeedingProbability( const unsigned sizeClassIndex ) {
    return mSizeClassFeedingProbabilities[ sizeClassIndex ];
}

void HeterotrophData::SetCoupledSizeClassIndex( const unsigned sizeClassIndex, const unsigned coupledIndex ) {
    mSizeClassCouplings[ sizeClassIndex ] = coupledIndex;
}

void HeterotrophData::SetEffectivePreyVolume( const unsigned sizeClassIndex, const double effectivePreyVolume ) {
    mSizeClassEffectivePreyVolumes[ sizeClassIndex ] = effectivePreyVolume;
}

void HeterotrophData::SetFeedingProbability( const unsigned sizeClassIndex, const double feedingProbability ) {
    mSizeClassFeedingProbabilities[ sizeClassIndex ] = feedingProbability;
}

void HeterotrophData::AddIndividualData( const Types::IndividualPointer individual ) {
    AddTrophicLevel( individual->GetTrophicLevel( ), individual->GetVolumeActual( ), individual->GetSizeClassIndex( ), individual->GetAge( ) );
    mSizeClassGrowthRatios[ individual->GetSizeClassIndex( ) ] += ( individual->GetVolumeActual( ) / individual->GetVolumeHeritable( ) );
    mSizeClassVolumes[ individual->GetSizeClassIndex( ) ] += individual->GetVolumeActual( );
    mVolume += individual->GetVolumeActual( );
    mSizeClassAges[ individual->GetSizeClassIndex( ) ] += individual->GetAge( );
}

void HeterotrophData::AddSizeClassData( const unsigned sizeClassIndex, const unsigned sizeClassPopulation, const double sizeClassMultiplier ) {
    mFrequency += sizeClassPopulation;

    double sizeClassVolumeApproximation = Parameters::Get( )->GetSizeClassMidPoint( sizeClassIndex ) * sizeClassPopulation;
    mApproxVolume += sizeClassVolumeApproximation;
    if( sizeClassPopulation > 0 ) {
        mSizeClassPopulation[ sizeClassIndex ] = sizeClassPopulation;
        mSizeClassApproxVolumes[ sizeClassIndex ] = sizeClassVolumeApproximation;
        mSizeClassGrowthRatios[ sizeClassIndex ] = mSizeClassGrowthRatios[ sizeClassIndex ] * sizeClassMultiplier;
        mSizeClassTrophicClassifications[ sizeClassIndex ] = mSizeClassTrophicClassifications[ sizeClassIndex ] * sizeClassMultiplier;
        mSizeClassAges[ sizeClassIndex ] = mSizeClassAges[ sizeClassIndex ] * sizeClassMultiplier;
    } else {
        mSizeClassVolumes[ sizeClassIndex ] = Constants::cMissingValue;
        mSizeClassGrowthRatios[ sizeClassIndex ] = Constants::cMissingValue;
        mSizeClassTrophicClassifications[ sizeClassIndex ] = Constants::cMissingValue;
        mSizeClassAges[ sizeClassIndex ] = Constants::cMissingValue;
    }
}

void HeterotrophData::AddTrophicLevel( const double trophicLevel, const double volumeActual, const unsigned sizeClassIndex, const unsigned age ) {
    for( unsigned trophicIndex = 0; trophicIndex <= Constants::cMaximumNumberOfTrophicLevels; ++trophicIndex ) {
        if( trophicLevel < ( trophicIndex + 0.5 ) ) {
            unsigned discreteTrophicLevel = trophicIndex;
            ++mTrophicFrequencies[ discreteTrophicLevel ];
            mTrophicVolumes[ discreteTrophicLevel ] += volumeActual;
            mTrophicAges[ discreteTrophicLevel ] += age;
            break;
        }
    }
    mSizeClassTrophicClassifications[ sizeClassIndex ] += trophicLevel;
}

void HeterotrophData::NormaliseData( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        if( mSizeClassPreyVolumeRatios[ sizeClassIndex ] > 0 ) {
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = mSizeClassPreyVolumeRatios[ sizeClassIndex ] / ( mSizeClassCarnivoreFrequencies[ sizeClassIndex ] + mSizeClassHerbivoreFrequencies[ sizeClassIndex ] );
        } else
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = Constants::cMissingValue;
    }
    for( unsigned trophicIndex = 0; trophicIndex < Constants::cMaximumNumberOfTrophicLevels; ++trophicIndex ) {
        if( mTrophicFrequencies[ trophicIndex ] > 0 )
            mTrophicAges[ trophicIndex ] = mTrophicAges[ trophicIndex ] / ( double ) mTrophicFrequencies[ trophicIndex ];
    }
}

bool HeterotrophData::AreHeterotrophsAlive( ) const {
    return mVolume > Parameters::Get( )->GetMinimumHeterotrophVolume( );
}

void HeterotrophData::ResetDataStructures( ) {
    mToFlux = 0;
    mInFlux = 0;

    mTimeFeeding = 0;
    mTimeMetabolising = 0;
    mTimeReproducing = 0;
    mTimeStarving = 0;

    mSizeClassHerbivoreFrequencies.clear( );
    mSizeClassPreyFrequencies.clear( );
    mSizeClassCarnivoreFrequencies.clear( );
    mSizeClassStarvedFrequencies.clear( );
    mSizeClassParentFrequencies.clear( );
    mSizeClassChildFrequencies.clear( );
    mSizeClassVolumeMutantFrequencies.clear( );

    mSizeClassPreyVolumeRatios.clear( );

    mSizeClassEffectivePreyVolumes.clear( );
    mSizeClassFeedingProbabilities.clear( );
    mSizeClassCouplings.clear( );

    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );
    mSizeClassHerbivoreFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassPreyFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassCarnivoreFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassStarvedFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassParentFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassChildFrequencies.resize( numberOfSizeClasses, 0 );
    mSizeClassVolumeMutantFrequencies.resize( numberOfSizeClasses, 0 );

    mSizeClassPreyVolumeRatios.resize( numberOfSizeClasses, 0 );

    mSizeClassEffectivePreyVolumes.resize( numberOfSizeClasses, Constants::cMissingValue );
    mSizeClassFeedingProbabilities.resize( numberOfSizeClasses, Constants::cMissingValue );
    mSizeClassCouplings.resize( numberOfSizeClasses, Constants::cMissingValue );

    mEffectiveSizeClassVolumeMatrix.resize( numberOfSizeClasses );
}

void HeterotrophData::IncrementVegetarianFrequencies( const Types::IndividualPointer grazer ) {
    ++mSizeClassHerbivoreFrequencies[ grazer->GetSizeClassIndex( ) ];

    double preyVolumeRatio = grazer->GetVolumeActual( ) / Parameters::Get( )->GetSmallestIndividualVolume( );
    mSizeClassPreyVolumeRatios[ grazer->GetSizeClassIndex( ) ] = mSizeClassPreyVolumeRatios[ grazer->GetSizeClassIndex( ) ] + preyVolumeRatio;
    mToFlux += Parameters::Get( )->GetSmallestIndividualVolume( );
}

void HeterotrophData::IncrementCarnivoreFrequencies( const Types::IndividualPointer predator, const Types::IndividualPointer prey ) {
    ++mSizeClassCarnivoreFrequencies[ predator->GetSizeClassIndex( ) ];
    ++mSizeClassPreyFrequencies[ prey->GetSizeClassIndex( ) ];

    double preyVolumeRatio = predator->GetVolumeActual( ) / prey->GetVolumeActual( );
    mSizeClassPreyVolumeRatios[ predator->GetSizeClassIndex( ) ] = mSizeClassPreyVolumeRatios[ predator->GetSizeClassIndex( ) ] + preyVolumeRatio;
    mInFlux += prey->GetVolumeActual( );
}

void HeterotrophData::IncrementStarvedFrequencies( const unsigned sizeClassIndex ) {
    ++mSizeClassStarvedFrequencies[ sizeClassIndex ];
}

void HeterotrophData::IncrementMutantFrequency( const unsigned sizeClassIndex, const unsigned geneIndex ) {
    if( geneIndex == Constants::eVolume )
        ++mSizeClassVolumeMutantFrequencies[ sizeClassIndex ];
}

void HeterotrophData::IncrementBirthFrequencies( const unsigned parentIndex, const unsigned childIndex ) {
    ++mSizeClassParentFrequencies[ parentIndex ];
    ++mSizeClassChildFrequencies[ childIndex ];
}

void HeterotrophData::AddToTimeFeeding( double timeFeeding ) {
    mTimeFeeding += timeFeeding;
}

void HeterotrophData::AddToTimeMetabolising( double timeMetabolising ) {
    mTimeMetabolising += timeMetabolising;
}

void HeterotrophData::AddToTimeReproducing( double timeReproducing ) {
    mTimeReproducing += timeReproducing;
}

void HeterotrophData::AddToTimeStarving( double timeStarving ) {
    mTimeStarving += timeStarving;
}