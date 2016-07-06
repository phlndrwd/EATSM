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

    mVegetarianFrequency = 0;
    mCarnivoreFrequency = 0;
    mFailedFeedingAttemptFrequency = 0;
    mFailedVegetarianFrequency = 0;
    mFailedCarnivoreFrequency = 0;
    mStarvedFrequency = 0;
    mBirthFrequency = 0;

    mEffectiveSizeClassVolumeMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mSizeClassInteractionProbabilityMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mEffectiveSizeClassVolumeMatrix[ sizeClassIndex ].resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
        mSizeClassInteractionProbabilityMatrix[ sizeClassIndex ].resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    }

    ResetDataStructures( );
}

HeterotrophData::~HeterotrophData( ) {

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mEffectiveSizeClassVolumeMatrix.clear( );
        mSizeClassInteractionProbabilityMatrix[ sizeClassIndex ].clear( );
    }
    mEffectiveSizeClassVolumeMatrix.clear( );
    mSizeClassInteractionProbabilityMatrix.clear( );

    mSizeClassSizes.clear( );
    mSizeClassVegetarianFrequencies.clear( );
    mSizeClassCarnivoreFrequencies.clear( );
    mSizeClassPreyFrequencies.clear( );
    mSizeClassStarvedFrequencies.clear( );
    mSizeClassParentFrequencies.clear( );
    mSizeClassChildFrequencies.clear( );
    mSizeClassVolumeMutantFrequencies.clear( );
    mSizeClassVolumes.clear( );
    mSizeClassApproximateVolumes.clear( );
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

    mSizeClassSizes.clear( );
    mSizeClassVolumes.clear( );
    mSizeClassApproximateVolumes.clear( );
    mSizeClassGrowthRatios.clear( );
    mSizeClassTrophicClassifications.clear( );
    mSizeClassAges.clear( );

    mSizeClassSizes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassVolumes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassApproximateVolumes.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassGrowthRatios.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassTrophicClassifications.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mSizeClassAges.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );

    mTrophicFrequencies.clear( );
    mTrophicVolumes.clear( );
    mTrophicAges.clear( );

    mTrophicFrequencies.resize( Constants::cMaximumNumberOfTrophicLevels, 0 ); // 0 = unclassified, 1 = phytoplankton, 2 = secondary producer
    mTrophicVolumes.resize( Constants::cMaximumNumberOfTrophicLevels, 0 );
    mTrophicAges.resize( Constants::cMaximumNumberOfTrophicLevels, 0 );

    mVolume = 0;
    mApproximateVolume = 0;

    mFrequency = 0;
}

void HeterotrophData::RecordOutputData( ) {

    // Vector datums
    DataRecorder::Get( )->AddIntegerVectorData( Constants::eHeterotrophFrequency, Constants::cVectorDatumNames[ Constants::eHeterotrophFrequency ], mFrequency );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eHeterotrophVolume, Constants::cVectorDatumNames[ Constants::eHeterotrophVolume ], mVolume );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eHeterotrophApproximateVolume, Constants::cVectorDatumNames[ Constants::eHeterotrophApproximateVolume ], mApproximateVolume );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eToHeterotrophFlux, Constants::cVectorDatumNames[ Constants::eToHeterotrophFlux ], mToFlux );
    DataRecorder::Get( )->AddFloatVectorData( Constants::eInHeterotrophFlux, Constants::cVectorDatumNames[ Constants::eInHeterotrophFlux ], mInFlux );

    // Matrix datums
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassSizes, Constants::cSizeClassDatumNames[ Constants::eSizeClassSizes ], mSizeClassSizes );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassVegetarianFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassVegetarianFrequencies ], mSizeClassVegetarianFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassCarnivoreFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassCarnivoreFrequencies ], mSizeClassCarnivoreFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassPreyFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassPreyFrequencies ], mSizeClassPreyFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassStarvedFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassStarvedFrequencies ], mSizeClassStarvedFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassParentFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassParentFrequencies ], mSizeClassParentFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassChildFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassChildFrequencies ], mSizeClassChildFrequencies );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassVolumeMutantFrequencies, Constants::cSizeClassDatumNames[ Constants::eSizeClassVolumeMutantFrequencies ], mSizeClassVolumeMutantFrequencies );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassVolumes, Constants::cSizeClassDatumNames[ Constants::eSizeClassVolumes ], mSizeClassVolumes );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassApproximateVolumes, Constants::cSizeClassDatumNames[ Constants::eSizeClassApproximateVolumes ], mSizeClassApproximateVolumes );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassEffectivePreyVolumes, Constants::cSizeClassDatumNames[ Constants::eSizeClassEffectivePreyVolumes ], mSizeClassEffectivePreyVolumes );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassGrowthRatios, Constants::cSizeClassDatumNames[ Constants::eSizeClassGrowthRatios ], mSizeClassGrowthRatios );
    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eSizeClassCouplings, Constants::cSizeClassDatumNames[ Constants::eSizeClassCouplings ], mSizeClassCouplings );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassPreyVolumeRatios, Constants::cSizeClassDatumNames[ Constants::eSizeClassPreyVolumeRatios ], mSizeClassPreyVolumeRatios );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassFeedingProbabilities, Constants::cSizeClassDatumNames[ Constants::eSizeClassFeedingProbabilities ], mSizeClassFeedingProbabilities );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassTrophicClassifications, Constants::cSizeClassDatumNames[ Constants::eSizeClassTrophicClassifications ], mSizeClassTrophicClassifications );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eSizeClassAges, Constants::cSizeClassDatumNames[ Constants::eSizeClassAges ], mSizeClassAges );

    DataRecorder::Get( )->AddIntegerMatrixData( Constants::eTrophicFrequencies, Constants::cTrophicDatumNames[ Constants::eTrophicFrequencies ], mTrophicFrequencies );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eTrophicVolumes, Constants::cTrophicDatumNames[ Constants::eTrophicVolumes ], mTrophicVolumes );
    DataRecorder::Get( )->AddFloatMatrixData( Constants::eTrophicAges, Constants::cTrophicDatumNames[ Constants::eTrophicAges ], mTrophicAges );

    ResetDataStructures( );
}

void HeterotrophData::OutputSummary( ) {
    Logger::Get( )->LogString( "Successful vegetarians = " + Convertor::Get( )->NumberToString( mVegetarianFrequency ) );
    Logger::Get( )->LogString( "Successful carnivores  = " + Convertor::Get( )->NumberToString( mCarnivoreFrequency ) );
    Logger::Get( )->LogString( "Number reproduced = " + Convertor::Get( )->NumberToString( mBirthFrequency ) );
    Logger::Get( )->LogString( "Number starved = " + Convertor::Get( )->NumberToString( mStarvedFrequency ) );
    Logger::Get( )->LogString( "Failed carnivores = " + Convertor::Get( )->NumberToString( mFailedCarnivoreFrequency ) );
    Logger::Get( )->LogString( "Failed vegetarians = " + Convertor::Get( )->NumberToString( mFailedVegetarianFrequency ) );
    Logger::Get( )->LogString( "Failed feeding attempts = " + Convertor::Get( )->NumberToString( mFailedFeedingAttemptFrequency ) );
}

unsigned int HeterotrophData::GetProbabilisticPreySizeClassIndex( const unsigned int predatorIndex ) const {

    unsigned int randomPreySizeClassIndex = 0;

    double randomValue = RandomInterface::Get( )->GetUniformDouble( );

    double probabilitySum = 0;

    for( unsigned int preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {

        probabilitySum += mSizeClassInteractionProbabilityMatrix[ predatorIndex ][ preyIndex ];

        if( randomValue <= probabilitySum ) {
            randomPreySizeClassIndex = preyIndex;
            break;
        }
    }

    return randomPreySizeClassIndex;
}

void HeterotrophData::CalculateSizeClassInteractionProbabilities( ) {

    for( unsigned int predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {
        for( unsigned int preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {

            if( mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] != 0 ) {

                mSizeClassInteractionProbabilityMatrix[ predatorIndex ][ preyIndex ] = mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] / mSizeClassEffectivePreyVolumes[ predatorIndex ];
            }
        }
    }
}

double HeterotrophData::GetEffectiveSizeClassVolume( const unsigned int predatorIndex, const unsigned int preyIndex ) const {
    return mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ];
}

void HeterotrophData::SetEffectiveSizeClassVolume( const unsigned int predatorIndex, const unsigned int preyIndex, const double effectiveSizeClassVolume ) {
    mEffectiveSizeClassVolumeMatrix[ predatorIndex ][ preyIndex ] = effectiveSizeClassVolume;
}

unsigned int HeterotrophData::GetCoupledSizeClassIndex( const unsigned int sizeClassIndex ) {
    return mSizeClassCouplings[ sizeClassIndex ];
}

double HeterotrophData::GetEffectivePreyVolume( const unsigned int sizeClassIndex ) {
    return mSizeClassEffectivePreyVolumes[ sizeClassIndex ];
}

double HeterotrophData::GetFeedingProbability( const unsigned int sizeClassIndex ) {
    return mSizeClassFeedingProbabilities[ sizeClassIndex ];
}

void HeterotrophData::SetCoupledSizeClassIndex( const unsigned int sizeClassIndex, const unsigned int coupledIndex ) {
    mSizeClassCouplings[ sizeClassIndex ] = coupledIndex;
}

void HeterotrophData::SetEffectivePreyVolume( const unsigned int sizeClassIndex, const double effectivePreyVolume ) {
    mSizeClassEffectivePreyVolumes[ sizeClassIndex ] = effectivePreyVolume;
}

void HeterotrophData::SetFeedingProbability( const unsigned int sizeClassIndex, const double feedingProbability ) {
    mSizeClassFeedingProbabilities[ sizeClassIndex ] = feedingProbability;
}

void HeterotrophData::AddIndividualData( const Types::IndividualPointer individual ) {

    AddTrophicLevel( individual->GetTrophicLevel( ), individual->GetVolumeActual( ), individual->GetSizeClassIndex( ), individual->GetAge( ) );

    mSizeClassGrowthRatios[ individual->GetSizeClassIndex( ) ] += ( individual->GetVolumeActual( ) / individual->GetVolumeHeritable( ) );
    mSizeClassVolumes[ individual->GetSizeClassIndex( ) ] += individual->GetVolumeActual( );
    mVolume += individual->GetVolumeActual( );
    mSizeClassAges[ individual->GetSizeClassIndex( ) ] += individual->GetAge( );
}

void HeterotrophData::AddSizeClassData( const unsigned int sizeClassIndex, const unsigned int sizeClassSize ) {

    mFrequency += sizeClassSize;

    double sizeClassVolumeApproximation = sizeClassSize * Parameters::Get( )->GetSizeClassMidPoint( sizeClassIndex );

    mSizeClassApproximateVolumes[ sizeClassIndex ] = sizeClassVolumeApproximation;
    mApproximateVolume += sizeClassVolumeApproximation;

    mSizeClassSizes[ sizeClassIndex ] = sizeClassSize;

    if( sizeClassSize > 0 ) {
        mSizeClassGrowthRatios[ sizeClassIndex ] = mSizeClassGrowthRatios[ sizeClassIndex ] / ( double )sizeClassSize;
        mSizeClassTrophicClassifications[ sizeClassIndex ] = mSizeClassTrophicClassifications[ sizeClassIndex ] / ( double )sizeClassSize;
        mSizeClassAges[ sizeClassIndex ] = mSizeClassAges[ sizeClassIndex ] / ( double )sizeClassSize;
    } else {
        mSizeClassGrowthRatios[ sizeClassIndex ] = Constants::cNaNValue;
        mSizeClassTrophicClassifications[ sizeClassIndex ] = Constants::cNaNValue;
        mSizeClassAges[ sizeClassIndex ] = Constants::cNaNValue;
    }
}

void HeterotrophData::AddTrophicLevel( const double trophicLevel, const double volumeActual, const unsigned int sizeClassIndex, const unsigned int age ) {

    for( unsigned int trophicIndex = 0; trophicIndex <= Constants::cMaximumNumberOfTrophicLevels; ++trophicIndex ) {

        if( trophicLevel < ( trophicIndex + 0.5 ) ) {

            unsigned int discreteTrophicLevel = trophicIndex;

            ++mTrophicFrequencies[ discreteTrophicLevel ];
            mTrophicVolumes[ discreteTrophicLevel ] += volumeActual;
            mTrophicAges[ discreteTrophicLevel ] += age;
            break;
        }
    }

    mSizeClassTrophicClassifications[ sizeClassIndex ] += trophicLevel;
}

void HeterotrophData::NormaliseData( ) {

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        if( mSizeClassPreyVolumeRatios[ sizeClassIndex ] > 0 ) {
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = mSizeClassPreyVolumeRatios[ sizeClassIndex ] / ( mSizeClassCarnivoreFrequencies[ sizeClassIndex ] + mSizeClassVegetarianFrequencies[ sizeClassIndex ] );
        } else {
            mSizeClassPreyVolumeRatios[ sizeClassIndex ] = Constants::cNaNValue;
        }
    }

    for( unsigned int trophicIndex = 0; trophicIndex < Constants::cMaximumNumberOfTrophicLevels; ++trophicIndex ) {
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
    mSizeClassFeedingProbabilities.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cNaNValue );
    mSizeClassCouplings.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), Constants::cNaNValue );

    mEffectiveSizeClassVolumeMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mSizeClassInteractionProbabilityMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
}

void HeterotrophData::IncrementVegetarianFrequencies( const Types::IndividualPointer grazer ) {
    ++mSizeClassVegetarianFrequencies[ grazer->GetSizeClassIndex( ) ];

    double preyVolumeRatio = grazer->GetVolumeActual( ) / Parameters::Get( )->GetSmallestIndividualVolume( );

    mSizeClassPreyVolumeRatios[ grazer->GetSizeClassIndex( ) ] = mSizeClassPreyVolumeRatios[ grazer->GetSizeClassIndex( ) ] + preyVolumeRatio;

    ++mVegetarianFrequency;

    mToFlux += Parameters::Get( )->GetSmallestIndividualVolume( );
}

void HeterotrophData::IncrementCarnivoreFrequencies( const Types::IndividualPointer predator, const Types::IndividualPointer prey ) {
    ++mSizeClassCarnivoreFrequencies[ predator->GetSizeClassIndex( ) ];
    ++mSizeClassPreyFrequencies[ prey->GetSizeClassIndex( ) ];

    double preyVolumeRatio = predator->GetVolumeActual( ) / prey->GetVolumeActual( );

    mSizeClassPreyVolumeRatios[ predator->GetSizeClassIndex( ) ] = mSizeClassPreyVolumeRatios[ predator->GetSizeClassIndex( ) ] + preyVolumeRatio;

    ++mCarnivoreFrequency;

    mInFlux += prey->GetVolumeActual( );
}

void HeterotrophData::IncrementFailedFeedingAttemptFrequency( ) {
    ++mFailedFeedingAttemptFrequency;
}

void HeterotrophData::IncrementFailedVegetarianFrequency( ) {
    ++mFailedVegetarianFrequency;
}

void HeterotrophData::IncrementFailedCarnivoreFrequency( ) {
    ++mFailedCarnivoreFrequency;
}

void HeterotrophData::IncrementStarvedFrequencies( const unsigned int sizeClassIndex ) {
    ++mSizeClassStarvedFrequencies[ sizeClassIndex ];
    ++mStarvedFrequency;
}

void HeterotrophData::IncrementMutantFrequency( const unsigned int sizeClassIndex, const unsigned int geneIndex ) {

    if( geneIndex == Constants::eVolumeGene ) {
        ++mSizeClassVolumeMutantFrequencies[ sizeClassIndex ];
    }
}

void HeterotrophData::IncrementBirthFrequencies( const unsigned int parentIndex, const unsigned int childIndex ) {
    ++mSizeClassParentFrequencies[ parentIndex ];
    ++mSizeClassChildFrequencies[ childIndex ];
    ++mBirthFrequency;
}

unsigned int HeterotrophData::GetVegetarianFrequency( ) const {
    return mVegetarianFrequency;
}

unsigned int HeterotrophData::GetCarnivoreFrequency( ) const {
    return mCarnivoreFrequency;
}

unsigned int HeterotrophData::GetChildFrequency( ) const {
    return mBirthFrequency;
}

unsigned int HeterotrophData::GetStarvedFrequency( ) const {
    return mStarvedFrequency;
}

unsigned int HeterotrophData::GetFailedCarnivoreFrequency( ) const {
    return mFailedCarnivoreFrequency;
}

unsigned int HeterotrophData::GeFailedVegetarianFrequency( ) const {
    return mFailedVegetarianFrequency;
}

unsigned int HeterotrophData::GetFailedFeedingAttemptFrequency( ) const {
    return mFailedFeedingAttemptFrequency;
}
