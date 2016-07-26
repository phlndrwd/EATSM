#include "HeterotrophData.h"

#include "DataRecorder.h"
#include "Parameters.h"
#include "Logger.h"
#include "Convertor.h"
#include "Individual.h"
#include "Maths.h"
#include "Genome.h"
#include "RandomInterface.h"

HeterotrophData::HeterotrophData( ) {
    
    DataRecorder::Get( )->SetVectorDataOn( "AxisSizeClassMidPointValues", Parameters::Get( )->GetSizeClassMidPoints( ) );
    DataRecorder::Get( )->SetVectorDataOn( "AxisSizeClassBoundaryValues", Parameters::Get( )->GetSizeClassBoundaries( ) );
    
    mEffectiveSizeClassVolumeMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mSizeClassInteractionProbabilityMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mEffectiveSizeClassVolumeMatrix[ sizeClassIndex ].resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
        mSizeClassInteractionProbabilityMatrix[ sizeClassIndex ].resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    }

    ResetDataStructures( );
}

HeterotrophData::~HeterotrophData( ) {

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mEffectiveSizeClassVolumeMatrix.clear( );
        mSizeClassInteractionProbabilityMatrix[ sizeClassIndex ].clear( );
    }
    mEffectiveSizeClassVolumeMatrix.clear( );
    mSizeClassInteractionProbabilityMatrix.clear( );

    mSizeClassPopulation.clear( );
    mSizeClassVegetarianFrequencies.clear( );
    mSizeClassCarnivoreFrequencies.clear( );
    mSizeClassPreyFrequencies.clear( );
    mSizeClassStarvedFrequencies.clear( );
    mSizeClassParentFrequencies.clear( );
    mSizeClassChildFrequencies.clear( );
    mSizeClassVolumeMutantFrequencies.clear( );
    mSizeClassVolumes.clear( );
    mSizeClassApproxVolumes.clear( );
    mSizeClassEffectivePreyVolumes.clear( );

    mSizeClassGrowthRatios.clear( );
    mSizeClassCouplings.clear( );
    mSizeClassPreyVolumeRatios.clear( );
    mSizeClassFeedingProbabilities.clear( );
    mSizeClassTrophicClassifications.clear( );
    mSizeClassAges.clear( );

    mTrophicFrequencies.clear( );
    mTrophicVolumes.clear( );
    mTrophicAges.clear( );
}

void HeterotrophData::InitialiseDataStructures( ) {

    mSizeClassPopulation.clear( );
    mSizeClassVolumes.clear( );
    mSizeClassApproxVolumes.clear( );
    mSizeClassGrowthRatios.clear( );
    mSizeClassTrophicClassifications.clear( );
    mSizeClassAges.clear( );

    mSizeClassPopulation.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );
    mSizeClassVolumes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );
    mSizeClassApproxVolumes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );
    mSizeClassGrowthRatios.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassTrophicClassifications.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassAges.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );
    
    mTrophicFrequencies.clear( );
    mTrophicVolumes.clear( );
    mTrophicAges.clear( );

    mTrophicFrequencies.resize( Constants::cMaximumNumberOfTrophicLevels, Constants::cMissingValue ); // 0 = unclassified, 1 = phytoplankton, 2 = secondary producer
    mTrophicVolumes.resize( Constants::cMaximumNumberOfTrophicLevels, Constants::cMissingValue );
    mTrophicAges.resize( Constants::cMaximumNumberOfTrophicLevels, Constants::cMissingValue );

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

    DataRecorder::Get( )->AddDataTo( "SizeClassPopulation", mSizeClassPopulation );
    DataRecorder::Get( )->AddDataTo( "SizeClassVegetarianFrequencies", mSizeClassVegetarianFrequencies );
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

unsigned HeterotrophData::GetProbabilisticPreySizeClassIndex( const unsigned predatorIndex ) const {

    unsigned randomPreySizeClassIndex = 0;

    double randomValue = RandomInterface::Get( )->GetUniformDouble( );

    double probabilitySum = 0;

    for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {

        probabilitySum += mSizeClassInteractionProbabilityMatrix[ predatorIndex ][ preyIndex ];

        if( randomValue <= probabilitySum ) {
            randomPreySizeClassIndex = preyIndex;
            break;
        }
    }

    return randomPreySizeClassIndex;
}

void HeterotrophData::CalculateSizeClassInteractionProbabilities( ) {

    for( unsigned predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {
        for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {

            if( mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] != 0 ) {

                mSizeClassInteractionProbabilityMatrix[ predatorIndex ][ preyIndex ] = mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] / mSizeClassEffectivePreyVolumes[ predatorIndex ];
            }
        }
    }
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

void HeterotrophData::AddSizeClassData( const unsigned sizeClassIndex, const unsigned sizeClassSize ) {

    mFrequency += sizeClassSize;

    double sizeClassVolumeApproximation = sizeClassSize * Parameters::Get( )->GetSizeClassMidPoint( sizeClassIndex );

    mSizeClassApproxVolumes[ sizeClassIndex ] = sizeClassVolumeApproximation;
    mApproxVolume += sizeClassVolumeApproximation;

    mSizeClassPopulation[ sizeClassIndex ] = sizeClassSize;

    if( sizeClassSize > 0 ) {
        mSizeClassGrowthRatios[ sizeClassIndex ] = mSizeClassGrowthRatios[ sizeClassIndex ] / ( double )sizeClassSize;
        mSizeClassTrophicClassifications[ sizeClassIndex ] = mSizeClassTrophicClassifications[ sizeClassIndex ] / ( double )sizeClassSize;
        mSizeClassAges[ sizeClassIndex ] = mSizeClassAges[ sizeClassIndex ] / ( double )sizeClassSize;
    } else {
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
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = mSizeClassPreyVolumeRatios[ sizeClassIndex ] / ( mSizeClassCarnivoreFrequencies[ sizeClassIndex ] + mSizeClassVegetarianFrequencies[ sizeClassIndex ] );
        } else {
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = Constants::cMissingValue;
        }
    }

    for( unsigned trophicIndex = 0; trophicIndex < Constants::cMaximumNumberOfTrophicLevels; ++trophicIndex ) {
        if( mTrophicFrequencies[ trophicIndex ] > 0 ) {
            mTrophicAges[ trophicIndex ] = mTrophicAges[ trophicIndex ] / ( double )mTrophicFrequencies[ trophicIndex ];
        }
    }
}

bool HeterotrophData::AreHeterotrophsAlive( ) const {
    return mVolume > Parameters::Get( )->GetMinimumHeterotrophVolume( );
}

void HeterotrophData::ResetDataStructures( ) {

    mToFlux = 0;
    mInFlux = 0;

    mSizeClassVegetarianFrequencies.clear( );
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

    mSizeClassVegetarianFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassPreyFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassCarnivoreFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassStarvedFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassParentFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassChildFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassVolumeMutantFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );

    mSizeClassPreyVolumeRatios.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );

    mSizeClassEffectivePreyVolumes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassFeedingProbabilities.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );
    mSizeClassCouplings.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cMissingValue );

    mEffectiveSizeClassVolumeMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mSizeClassInteractionProbabilityMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
}

void HeterotrophData::IncrementVegetarianFrequencies( const Types::IndividualPointer grazer ) {
    ++mSizeClassVegetarianFrequencies[ grazer->GetSizeClassIndex( ) ];

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
    if( geneIndex == Constants::eVolumeGene ) {
        ++mSizeClassVolumeMutantFrequencies[ sizeClassIndex ];
    }
}

void HeterotrophData::IncrementBirthFrequencies( const unsigned parentIndex, const unsigned childIndex ) {
    ++mSizeClassParentFrequencies[ parentIndex ];
    ++mSizeClassChildFrequencies[ childIndex ];
}