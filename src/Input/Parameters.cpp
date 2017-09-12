#include "Parameters.h"
#include "Constants.h"
#include "Strings.h"
#include "HeterotrophProcessor.h"
#include "DataRecorder.h"

#include <cmath>

Types::ParametersPointer Parameters::mThis = NULL;

Parameters::Parameters( ) {

}

Parameters::~Parameters( ) {
    if( mThis != NULL ) {
        for( unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex ) {
            mInterSizeClassPreferenceMatrix[ sizeClassIndex ].clear( );
            mInterSizeClassVolumeMatrix[ sizeClassIndex ].clear( );
        }
        mInterSizeClassPreferenceMatrix.clear( );
        mInterSizeClassVolumeMatrix.clear( );

        mSizeClassBoundaries.clear( );
        mSizeClassMidPoints.clear( );

        delete mThis;
    }
}

Types::ParametersPointer Parameters::Get( ) {
    if( mThis == NULL ) mThis = new Parameters;

    return mThis;
}

bool Parameters::Initialise( const Types::StringMatrix& rawInputParameterData ) {
    if( rawInputParameterData.size( ) > 0 ) {
        for( unsigned rowIndex = 0; rowIndex < rawInputParameterData.size( ); ++rowIndex ) {

            std::string parameterName = Strings::Get( )->ToLowercase( rawInputParameterData[ rowIndex ][ Constants::eParameterName ] );
            double parameterValue = Strings::Get( )->StringToNumber( rawInputParameterData[ rowIndex ][ Constants::eParameterValue ] );

            if( parameterName == "randomseed" ) SetRandomSeed( parameterValue );
            else if( parameterName == "runtimeinseconds" ) SetRunTimeInSeconds( parameterValue );
            else if( parameterName == "samplingrate" ) SetSamplingRate( parameterValue );
            else if( parameterName == "numberofsizeclasses" ) SetNumberOfSizeClasses( parameterValue );

            else if( parameterName == "applystarvationfunction" ) SetApplyStarvationFunction( parameterValue );
            else if( parameterName == "readmodelstate" ) SetReadModelState( parameterValue );
            else if( parameterName == "writemodelstate" ) SetWriteModelState( parameterValue );

            else if( parameterName == "initialautotrophvolume" ) SetInitialAutotrophVolume( parameterValue );
            else if( parameterName == "initialheterotrophvolume" ) SetInitialHeterotrophVolume( parameterValue );
            else if( parameterName == "minimumheterotrophvolume" ) SetMinimumHeterotrophVolume( parameterValue );

            else if( parameterName == "smallestindividualvolume" ) SetSmallestIndividualVolume( parameterValue );
            else if( parameterName == "largestindividualvolume" ) SetLargestIndividualVolume( parameterValue );
            else if( parameterName == "preferredpreyvolumeratio" ) SetPreferredPreyVolumeRatio( parameterValue );
            else if( parameterName == "preferencefunctionwidth" ) SetPreferenceFunctionWidth( parameterValue );
            else if( parameterName == "sizeclasssubsetfraction" ) SetSizeClassSubsetFraction( parameterValue );
            else if( parameterName == "halfsaturationconstantfraction" ) SetHalfSaturationConstantFraction( parameterValue );

            else if( parameterName == "assimilationefficiency" ) SetAssimilationEfficiency( parameterValue );
            else if( parameterName == "fractionalmetabolicexpense" ) SetFractionalMetabolicExpense( parameterValue );

            else if( parameterName == "metabolicindex" ) SetMetabolicIndex( parameterValue );
            else if( parameterName == "mutationprobability" ) SetMutationProbability( parameterValue );
            else if( parameterName == "mutationstandarddeviation" ) SetMutationStandardDeviation( parameterValue );
        }
        CalculateParameters( );

        return true;
    } else {
        return false;
    }
}

void Parameters::CalculateParameters( ) {
    mSizeClassMidPoints.resize( mNumberOfSizeClasses );
    mSizeClassBoundaries.resize( mNumberOfSizeClasses + 1 );

    mSmallestVolumeExponent = std::log10( mSmallestIndividualVolume );
    mLargestVolumeExponent = std::log10( mLargestIndividualVolume );

    double sizeClassExponentIncrement = ( mLargestVolumeExponent - mSmallestVolumeExponent ) / mNumberOfSizeClasses;

    for( unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex ) {
        double sizeClassMidPointExponent = mSmallestVolumeExponent + ( ( sizeClassIndex + 0.5 ) * sizeClassExponentIncrement );
        double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( sizeClassIndex * sizeClassExponentIncrement );

        mSizeClassBoundaries[ sizeClassIndex ] = std::pow( 10, sizeClassBoundaryExponent );
        mSizeClassMidPoints[ sizeClassIndex ] = std::pow( 10, sizeClassMidPointExponent );
    }

    double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( mNumberOfSizeClasses * sizeClassExponentIncrement );

    mSizeClassBoundaries[ mNumberOfSizeClasses ] = std::pow( 10, sizeClassBoundaryExponent );

    mInterSizeClassPreferenceMatrix.resize( mNumberOfSizeClasses );
    mInterSizeClassVolumeMatrix.resize( mNumberOfSizeClasses );

    Types::HeterotrophProcessorPointer temporaryHeterotrophProcessor = new HeterotrophProcessor( );

    mPhytoplanktonSizeClassIndex = temporaryHeterotrophProcessor->FindSizeClassIndexFromVolume( mSmallestIndividualVolume );

    mTotalVolume = mInitialAutotrophVolume + mInitialHeterotrophVolume;
    mHalfSaturationConstant = mHalfSaturationConstantFraction * mTotalVolume;

    for( unsigned subjectIndex = 0; subjectIndex < mNumberOfSizeClasses; ++subjectIndex ) {
        double subjectVolumeMean = mSizeClassMidPoints[ subjectIndex ];
        double preferenceSum = 0;

        for( unsigned referenceIndex = 0; referenceIndex < mNumberOfSizeClasses; ++referenceIndex ) {
            double referenceVolumeMean = mSizeClassMidPoints[ referenceIndex ];

            double preferenceForReferenceSizeClass = temporaryHeterotrophProcessor->CalculatePreferenceForPrey( subjectVolumeMean, referenceVolumeMean );

            preferenceSum += preferenceForReferenceSizeClass;

            mInterSizeClassPreferenceMatrix[ subjectIndex ].push_back( preferenceForReferenceSizeClass );
            mInterSizeClassVolumeMatrix[ subjectIndex ].push_back( preferenceForReferenceSizeClass * referenceVolumeMean );
        }
    }
    delete temporaryHeterotrophProcessor;
}

unsigned& Parameters::GetRunTimeInSeconds( ) {
    return mRunTimeInSeconds;
}

unsigned& Parameters::GetRandomSeed( ) {
    return mRandomSeed;
}

unsigned& Parameters::GetSamplingRate( ) {
    return mSamplingRate;
}

unsigned& Parameters::GetNumberOfSizeClasses( ) {
    return mNumberOfSizeClasses;
}

bool Parameters::GetReadModelState( ) {
    return mReadModelState;
}

bool Parameters::GetApplyStarvationFunction( ) {
    return mApplyStarvationFunction;
}

bool Parameters::GetWriteModelState( ) {
    return mWriteModelState;
}

double& Parameters::GetInitialAutotrophVolume( ) {
    return mInitialAutotrophVolume;
}

double& Parameters::GetInitialHeterotrophVolume( ) {
    return mInitialHeterotrophVolume;
}

double& Parameters::GetMinimumHeterotrophVolume( ) {
    return mMinimumHeterotrophVolume;
}

double& Parameters::GetSmallestIndividualVolume( ) {
    return mSmallestIndividualVolume;
}

double& Parameters::GetLargestIndividualVolume( ) {
    return mLargestIndividualVolume;
}

unsigned& Parameters::GetPreferredPreyVolumeRatio( ) {
    return mPreferredPreyVolumeRatio;
}

double& Parameters::GetPreferenceFunctionWidth( ) {
    return mPreferenceFunctionWidth;
}

double& Parameters::GetSizeClassSubsetFraction( ) {
    return mSizeClassSubsetFraction;
}

double& Parameters::GetHalfSaturationConstantFraction( ) {
    return mHalfSaturationConstantFraction;
}

double& Parameters::GetAssimilationEfficiency( ) {
    return mAssimilationEfficiency;
}

double& Parameters::GetFractionalMetabolicExpense( ) {
    return mFractionalMetabolicExpense;
}

double& Parameters::GetMetabolicIndex( ) {
    return mMetabolicIndex;
}

double& Parameters::GetMutationProbability( ) {
    return mMutationProbability;
}

double& Parameters::GetMutationStandardDeviation( ) {
    return mMutationStandardDeviation;
}

unsigned& Parameters::GetPhytoplanktonSizeClassIndex( ) {
    return mPhytoplanktonSizeClassIndex;
}

double& Parameters::GetSmallestVolumeExponent( ) {
    return mSmallestVolumeExponent;
}

double& Parameters::GetLargestVolumeExponent( ) {
    return mLargestVolumeExponent;
}

double Parameters::GetSizeClassBoundary( const unsigned index ) const {
    return mSizeClassBoundaries[ index ];
}

double Parameters::GetSizeClassMidPoint( const unsigned index ) const {
    return mSizeClassMidPoints[ index ];
}

const Types::FloatVector& Parameters::GetSizeClassBoundaries( ) {
    return mSizeClassBoundaries;
}

const Types::FloatVector& Parameters::GetSizeClassMidPoints( ) {
    return mSizeClassMidPoints;
}

double Parameters::GetInterSizeClassPreference( const unsigned subjectIndex, const unsigned referenceIndex ) const {
    return mInterSizeClassPreferenceMatrix[ subjectIndex ][ referenceIndex ];
}

double Parameters::GetInterSizeClassVolume( const unsigned subjectIndex, const unsigned referenceIndex ) const {
    return mInterSizeClassVolumeMatrix[ subjectIndex ][ referenceIndex ];
}

double& Parameters::GetTotalVolume( ) {
    return mTotalVolume;
}

double& Parameters::GetHalfSaturationConstant( ) {
    return mHalfSaturationConstant;
}

const Types::FloatVector& Parameters::GetInterSizeClassPreferenceVector( const unsigned index ) const {
    return mInterSizeClassPreferenceMatrix[ index ];
}

const Types::FloatVector& Parameters::GetInterSizeClassVolumeVector( const unsigned index ) const {
    return mInterSizeClassVolumeMatrix[ index ];
}

void Parameters::SetRandomSeed( const unsigned randomNumberSeed ) {
    mRandomSeed = randomNumberSeed;
}

void Parameters::SetRunTimeInSeconds( const unsigned runTimeInSeconds ) {
    mRunTimeInSeconds = runTimeInSeconds;
}

void Parameters::SetSamplingRate( const unsigned samplingRate ) {
    mSamplingRate = samplingRate;
}

void Parameters::SetNumberOfSizeClasses( const unsigned numberOfSizeClasses ) {
    mNumberOfSizeClasses = numberOfSizeClasses;
}

void Parameters::SetReadModelState( const bool createNewPopulation ) {
    mReadModelState = createNewPopulation;
}

void Parameters::SetApplyStarvationFunction( const bool applyStarvationFunction ) {
    mApplyStarvationFunction = applyStarvationFunction;
}

void Parameters::SetWriteModelState( const bool writeModelState ) {
    mWriteModelState = writeModelState;
}

void Parameters::SetInitialAutotrophVolume( const double initialAutotrophVolume ) {
    mInitialAutotrophVolume = initialAutotrophVolume;
}

void Parameters::SetInitialHeterotrophVolume( const double initialHeterotrophVolume ) {
    mInitialHeterotrophVolume = initialHeterotrophVolume;
}

void Parameters::SetMinimumHeterotrophVolume( const double minimumHeterotrophVolume ) {
    mMinimumHeterotrophVolume = minimumHeterotrophVolume;
}

void Parameters::SetSmallestIndividualVolume( double smallestIndividualVolume ) {
    mSmallestIndividualVolume = smallestIndividualVolume;
}

void Parameters::SetLargestIndividualVolume( double largestIndividualVolume ) {
    mLargestIndividualVolume = largestIndividualVolume;
}

void Parameters::SetPreferredPreyVolumeRatio( const unsigned preferredPreyVolumeRatio ) {
    mPreferredPreyVolumeRatio = preferredPreyVolumeRatio;
}

void Parameters::SetPreferenceFunctionWidth( const double preferenceFunctionWidth ) {
    mPreferenceFunctionWidth = preferenceFunctionWidth;
}

void Parameters::SetSizeClassSubsetFraction( const double sizeClassSubsetFraction ) {
    mSizeClassSubsetFraction = sizeClassSubsetFraction;
}

void Parameters::SetHalfSaturationConstantFraction( const double halfSaturationConstantFraction ) {
    mHalfSaturationConstantFraction = halfSaturationConstantFraction;
}

void Parameters::SetAssimilationEfficiency( const double assimilationEfficiency ) {
    mAssimilationEfficiency = assimilationEfficiency;
}

void Parameters::SetFractionalMetabolicExpense( const double fractionalMetabolicExpense ) {
    mFractionalMetabolicExpense = fractionalMetabolicExpense;
}

void Parameters::SetMetabolicIndex( const double metabolicIndex ) {
    mMetabolicIndex = metabolicIndex;
}

void Parameters::SetMutationProbability( const double mutationProbability ) {
    mMutationProbability = mutationProbability;
}

void Parameters::SetMutationStandardDeviation( const double mutationStandardDeviation ) {
    mMutationStandardDeviation = mutationStandardDeviation;
}
