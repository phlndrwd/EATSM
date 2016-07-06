#include "Parameters.h"

#include "Constants.h"
#include "DateTime.h"
#include "Maths.h"
#include "Convertor.h"
#include "HeterotrophProcessor.h"
#include "Logger.h"
#include "DataRecorder.h"

Types::ParametersPointer Parameters::mThis = 0;

Parameters::Parameters( ) {
    mParameterValuesVector.resize( Constants::cNumberOfParameters );
}

Parameters::~Parameters( ) {
    if( mThis != 0 ) {
        for( unsigned int sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex ) {
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
    if( mThis == 0 ) {
        mThis = new Parameters;
    }
    return mThis;
}

void Parameters::SetConstants( ) {

    mExperimentName = "Development";
    mRandomSeed = 0;
    mRunTimeInSeconds = 60;
    mSamplingRate = 100;
    mNumberOfSizeClasses = 100;

    mHeterotrophInitialisationMethod = Constants::eMultiple;
    mPreferenceFunctionType = Constants::eNormal;
    mFunctionalResponseType = Constants::eType2;
    mStarvationFunctionType = Constants::eLinear;

    mInitialNutrientVolume = 0;
    mInitialPhytoplanktonVolume = 1000000;
    mInitialHeterotrophVolume = 1000000;
    mMinimumHeterotrophVolume = 5000;

    mSmallestIndividualVolume = 1;
    mLargestIndividualVolume = 1e+10;

    mPreferredPreyVolumeRatio = 15;
    mPreferenceFunctionWidth = 0.75;
    mPreferenceFunctionHeight = 1;

    mSizeClassSubsetFraction = 0.01;
    mHalfSaturationConstantFraction = 0.15;

    mAssimilationEfficiency = 0.5;
    mFractionalMetabolicExpensePerTimeStep = 0.00001;
    mMetabolicIndex = 0.67;

    mMutationProbability = 0.001;
    mMutationStandardDeviation = 0.01;
}

void Parameters::CalculateSizeClassVectors( ) {

    mSizeClassMidPoints.resize( mNumberOfSizeClasses );
    mSizeClassBoundaries.resize( mNumberOfSizeClasses + 1 );

    mSmallestVolumeExponent = Maths::Get( )->Log10( mSmallestIndividualVolume );
    mLargestVolumeExponent = Maths::Get( )->Log10( mLargestIndividualVolume );

    double sizeClassExponentIncrement = ( mLargestVolumeExponent - mSmallestVolumeExponent ) / mNumberOfSizeClasses;

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex ) {

        double sizeClassMidPointExponent = mSmallestVolumeExponent + ( ( sizeClassIndex + 0.5 ) * sizeClassExponentIncrement );
        double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( sizeClassIndex * sizeClassExponentIncrement );

        mSizeClassBoundaries[ sizeClassIndex ] = Maths::Get( )->ToThePower( 10, sizeClassBoundaryExponent );
        mSizeClassMidPoints[ sizeClassIndex ] = Maths::Get( )->ToThePower( 10, sizeClassMidPointExponent );
    }

    double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( mNumberOfSizeClasses * sizeClassExponentIncrement );

    mSizeClassBoundaries[ mNumberOfSizeClasses ] = Maths::Get( )->ToThePower( 10, sizeClassBoundaryExponent );
}

void Parameters::CalculateVariables( ) {

    mInterSizeClassPreferenceMatrix.resize( mNumberOfSizeClasses );
    mInterSizeClassVolumeMatrix.resize( mNumberOfSizeClasses );

    Types::HeterotrophProcessorPointer temporaryHeterotrophProcessor = new HeterotrophProcessor( );

    mPhytoplanktonSizeClassIndex = temporaryHeterotrophProcessor->FindSizeClassIndexFromVolume( mSmallestIndividualVolume );

    mTotalVolume = mInitialNutrientVolume + mInitialPhytoplanktonVolume + mInitialHeterotrophVolume;
    mHalfSaturationConstant = mHalfSaturationConstantFraction * mTotalVolume;

    for( unsigned int subjectIndex = 0; subjectIndex < mNumberOfSizeClasses; ++subjectIndex ) {

        double subjectVolumeMean = mSizeClassMidPoints[ subjectIndex ];
        double preferenceSum = 0;

        for( unsigned int referenceIndex = 0; referenceIndex < mNumberOfSizeClasses; ++referenceIndex ) {

            double referenceVolumeMean = mSizeClassMidPoints[ referenceIndex ];

            double preferenceForReferenceSizeClass = temporaryHeterotrophProcessor->CalculatePreferenceForPrey( subjectVolumeMean, referenceVolumeMean );

            preferenceSum += preferenceForReferenceSizeClass;

            mInterSizeClassPreferenceMatrix[ subjectIndex ].push_back( preferenceForReferenceSizeClass );
            mInterSizeClassVolumeMatrix[ subjectIndex ].push_back( preferenceForReferenceSizeClass * referenceVolumeMean );
        }
    }

    delete temporaryHeterotrophProcessor;
}

double Parameters::GetParameterValueFromIndex( const unsigned int index ) {
    return mParameterValuesVector[ index ];
}

const std::string Parameters::GetExperimentName( ) const {
    return mExperimentName;
}

unsigned int Parameters::GetRunTimeInSeconds( ) const {
    return mRunTimeInSeconds;
}

unsigned int Parameters::GetRandomSeed( ) const {
    return mRandomSeed;
}

unsigned int Parameters::GetSamplingRate( ) const {
    return mSamplingRate;
}

unsigned int Parameters::GetNumberOfSizeClasses( ) const {
    return mNumberOfSizeClasses;
}

unsigned int Parameters::GetHeterotrophInitialisationMethod( ) const {
    return mHeterotrophInitialisationMethod;
}

unsigned int Parameters::GetPreferenceFunctionType( ) const {
    return mPreferenceFunctionType;
}

unsigned int Parameters::GetFunctionalResponseType( ) const {
    return mFunctionalResponseType;
}

unsigned int Parameters::GetStarvationFunctionType( ) const {
    return mStarvationFunctionType;
}

double Parameters::GetInitialNutrientVolume( ) const {
    return mInitialNutrientVolume;
}

double Parameters::GetInitialPhytoplanktonVolume( ) const {
    return mInitialPhytoplanktonVolume;
}

double Parameters::GetInitialHeterotrophVolume( ) const {
    return mInitialHeterotrophVolume;
}

double Parameters::GetMinimumHeterotrophVolume( ) const {
    return mMinimumHeterotrophVolume;
}

double Parameters::GetSmallestIndividualVolume( ) const {
    return mSmallestIndividualVolume;
}

double Parameters::GetLargestIndividualVolume( ) const {
    return mLargestIndividualVolume;
}

unsigned int Parameters::GetPreferredPreyVolumeRatio( ) const {
    return mPreferredPreyVolumeRatio;
}

double Parameters::GetPreferenceFunctionWidth( ) const {
    return mPreferenceFunctionWidth;
}

double Parameters::GetPreferenceFunctionHeight( ) const {
    return mPreferenceFunctionHeight;
}

double Parameters::GetSizeClassSubsetFraction( ) const {
    return mSizeClassSubsetFraction;
}

double Parameters::GetHalfSaturationConstantFraction( ) const {
    return mHalfSaturationConstantFraction;
}

double Parameters::GetAssimilationEfficiency( ) const {
    return mAssimilationEfficiency;
}

double Parameters::GetFractionalMetabolicExpensePerTimeStep( ) const {
    return mFractionalMetabolicExpensePerTimeStep;
}

double Parameters::GetMetabolicIndex( ) const {
    return mMetabolicIndex;
}

double Parameters::GetMutationProbability( ) const {
    return mMutationProbability;
}

double Parameters::GetMutationStandardDeviation( ) const {
    return mMutationStandardDeviation;
}

unsigned int Parameters::GetPhytoplanktonSizeClassIndex( ) const {
    return mPhytoplanktonSizeClassIndex;
}

double Parameters::GetSmallestVolumeExponent( ) const {
    return mSmallestVolumeExponent;
}

double Parameters::GetLargestVolumeExponent( ) const {
    return mLargestVolumeExponent;
}

double Parameters::GetSizeClassBoundary( const unsigned int index ) const {
    return mSizeClassBoundaries[ index ];
}

double Parameters::GetSizeClassMidPoint( const unsigned int index ) const {
    return mSizeClassMidPoints[ index ];
}

const Types::FloatVector Parameters::GetSizeClassBoundaries( ) const {
    return mSizeClassBoundaries;
}

const Types::FloatVector Parameters::GetSizeClassMidPoints( ) const {
    return mSizeClassMidPoints;
}

double Parameters::GetInterSizeClassPreference( const unsigned int subjectIndex, const unsigned int referenceIndex ) const {
    return mInterSizeClassPreferenceMatrix[ subjectIndex ][ referenceIndex ];
}

double Parameters::GetInterSizeClassVolume( const unsigned int subjectIndex, const unsigned int referenceIndex ) const {
    return mInterSizeClassVolumeMatrix[ subjectIndex ][ referenceIndex ];
}

double Parameters::GetTotalVolume( ) const {
    return mTotalVolume;
}

double Parameters::GetHalfSaturationConstant( ) const {
    return mHalfSaturationConstant;
}

const Types::FloatVector Parameters::GetInterSizeClassPreferenceVector( const unsigned int index ) const {
    return mInterSizeClassPreferenceMatrix[ index ];
}

const Types::FloatVector Parameters::GetInterSizeClassVolumeVector( const unsigned int index ) const {
    return mInterSizeClassVolumeMatrix[ index ];
}

// Setters

void Parameters::SetExperimentName( const std::string experimentName ) {
    mExperimentName = experimentName;
}

void Parameters::SetRandomSeed( const unsigned int randomNumberSeed ) {
    mRandomSeed = randomNumberSeed;
    mParameterValuesVector[ Constants::eRandomNumberSeed ] = randomNumberSeed;
}

void Parameters::SetRunTimeInSeconds( const unsigned int runTimeInSeconds ) {
    mRunTimeInSeconds = runTimeInSeconds;
    mParameterValuesVector[ Constants::eRunTimeInSeconds ] = runTimeInSeconds;
}

void Parameters::SetSamplingRate( const unsigned int samplingRate ) {
    mSamplingRate = samplingRate;
    mParameterValuesVector[ Constants::eSamplingRate ] = samplingRate;
}

void Parameters::SetNumberOfSizeClasses( const unsigned int numberOfSizeClasses ) {
    mNumberOfSizeClasses = numberOfSizeClasses;
    mParameterValuesVector[ Constants::eNumberOfSizeClasses ] = numberOfSizeClasses;
}

void Parameters::SetHeterotrophInitialisationMethod( const unsigned int heterotrophInitialisationMethod ) {
    mHeterotrophInitialisationMethod = heterotrophInitialisationMethod;
    mParameterValuesVector[ Constants::eHeterotrophInitialisationMethod ] = heterotrophInitialisationMethod;
}

void Parameters::SetPreferenceFunctionType( const unsigned int preferenceFunctionType ) {
    mPreferenceFunctionType = preferenceFunctionType;
    mParameterValuesVector[ Constants::ePreferenceFunctionType ] = preferenceFunctionType;
}

void Parameters::SetFunctionalResponseType( const unsigned int functionalResponseType ) {
    mFunctionalResponseType = functionalResponseType;
    mParameterValuesVector[ Constants::eFunctionalResponseType ] = functionalResponseType;
}

void Parameters::SetStarvationFunctionType( const unsigned int starvationFunctionType ) {
    mStarvationFunctionType = starvationFunctionType;
    mParameterValuesVector[ Constants::eStarvationFunctionType ] = starvationFunctionType;
}

void Parameters::SetInitialNutrientVolume( const double initialNutrientVolume ) {
    mInitialNutrientVolume = initialNutrientVolume;
    mParameterValuesVector[ Constants::eInitialNutrientVolume ] = initialNutrientVolume;
}

void Parameters::SetInitialPhytoplanktonVolume( const double initialPhytoplanktonVolume ) {
    mInitialPhytoplanktonVolume = initialPhytoplanktonVolume;
    mParameterValuesVector[ Constants::eInitialPhytoplanktonVolume ] = initialPhytoplanktonVolume;
}

void Parameters::SetInitialHeterotrophVolume( const double initialHeterotrophVolume ) {
    mInitialHeterotrophVolume = initialHeterotrophVolume;
    mParameterValuesVector[ Constants::eInitialHeterotrophVolume ] = initialHeterotrophVolume;
}

void Parameters::SetMinimumHeterotrophVolume( const double minimumHeterotrophVolume ) {
    mMinimumHeterotrophVolume = minimumHeterotrophVolume;
    mParameterValuesVector[ Constants::eMinimumHeterotrophVolume ] = minimumHeterotrophVolume;
}

void Parameters::SetSmallestIndividualVolume( double smallestIndividualVolume ) {
    mSmallestIndividualVolume = smallestIndividualVolume;
    mParameterValuesVector[ Constants::eSmallestIndividualVolume ] = smallestIndividualVolume;
}

void Parameters::SetLargestIndividualVolume( double largestIndividualVolume ) {
    mLargestIndividualVolume = largestIndividualVolume;
    mParameterValuesVector[ Constants::eLargestIndividualVolume ] = largestIndividualVolume;
}

void Parameters::SetPreferredPreyVolumeRatio( const unsigned int preferredPreyVolumeRatio ) {
    mPreferredPreyVolumeRatio = preferredPreyVolumeRatio;
    mParameterValuesVector[ Constants::ePreferredPreyVolumeRatio ] = preferredPreyVolumeRatio;
}

void Parameters::SetPreferenceFunctionWidth( const double preferenceFunctionWidth ) {
    mPreferenceFunctionWidth = preferenceFunctionWidth;
    mParameterValuesVector[ Constants::ePreferenceFunctionWidth ] = preferenceFunctionWidth;
}

void Parameters::SetPreferenceFunctionHeight( const double preferenceFunctionHeight ) {
    mPreferenceFunctionHeight = preferenceFunctionHeight;
    mParameterValuesVector[ Constants::ePreferenceFunctionHeight ] = preferenceFunctionHeight;
}

void Parameters::SetSizeClassSubsetFraction( const double sizeClassSubsetFraction ) {
    mSizeClassSubsetFraction = sizeClassSubsetFraction;
    mParameterValuesVector[ Constants::eSizeClassSubsetFraction ] = sizeClassSubsetFraction;
}

void Parameters::SetHalfSaturationConstantFraction( const double halfSaturationConstantFraction ) {
    mHalfSaturationConstantFraction = halfSaturationConstantFraction;
    mParameterValuesVector[ Constants::eHalfSaturationConstantFraction ] = halfSaturationConstantFraction;
}

void Parameters::SetAssimilationEfficiency( const double assimilationEfficiency ) {
    mAssimilationEfficiency = assimilationEfficiency;
    mParameterValuesVector[ Constants::eAssimilationEfficiency ] = assimilationEfficiency;
}

void Parameters::SetFractionalMetabolicExpensePerTimeStep( const double fractionalMetabolicExpensePerTimeStep ) {
    mFractionalMetabolicExpensePerTimeStep = fractionalMetabolicExpensePerTimeStep;
    mParameterValuesVector[ Constants::eFractionalMetabolicExpensePerTimeStep ] = fractionalMetabolicExpensePerTimeStep;
}

void Parameters::SetMetabolicIndex( const double metabolicIndex ) {
    mMetabolicIndex = metabolicIndex;
    mParameterValuesVector[ Constants::eMetabolicIndex ] = metabolicIndex;
}

void Parameters::SetMutationProbability( const double mutationProbability ) {
    mMutationProbability = mutationProbability;
    mParameterValuesVector[ Constants::eMutationProbability ] = mutationProbability;
}

void Parameters::SetMutationStandardDeviation( const double mutationStandardDeviation ) {
    mMutationStandardDeviation = mutationStandardDeviation;
    mParameterValuesVector[ Constants::eMutationStandardDeviation ] = mutationStandardDeviation;
}
