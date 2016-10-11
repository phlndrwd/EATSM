#include "Parameters.h"
#include "Constants.h"
#include "Maths.h"
#include "Convertor.h"
#include "HeterotrophProcessor.h"
#include "Logger.h"
#include "DataRecorder.h"

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

            std::string parameterName = Convertor::Get( )->RemoveWhiteSpace( Convertor::Get( )->ToLowercase( rawInputParameterData[ rowIndex ][ Constants::eParameterName ] ) );
            double parameterValue = Convertor::Get( )->StringToNumber( rawInputParameterData[ rowIndex ][ Constants::eParameterValue ] );

            if( parameterName == "randomseed" ) SetRandomSeed( parameterValue );
            else if( parameterName == "runtimeinseconds" ) SetRunTimeInSeconds( parameterValue );
            else if( parameterName == "samplingrate" ) SetSamplingRate( parameterValue );
            else if( parameterName == "numberofsizeclasses" ) SetNumberOfSizeClasses( parameterValue );

            else if( parameterName == "initialisationmethod" ) SetInitialisationMethod( parameterValue );
            else if( parameterName == "applystarvationfunction" ) SetApplyStarvationFunction( parameterValue );
            else if( parameterName == "writemodelstate" ) SetWriteModelState( parameterValue );

            else if( parameterName == "initialnutrientvolume" ) SetInitialNutrientVolume( parameterValue );
            else if( parameterName == "initialautotrophvolume" ) SetInitialAutotrophVolume( parameterValue );
            else if( parameterName == "initialheterotrophvolume" ) SetInitialHeterotrophVolume( parameterValue );
            else if( parameterName == "minimumheterotrophvolume" ) SetMinimumHeterotrophVolume( parameterValue );

            else if( parameterName == "smallestindividualvolume" ) SetSmallestIndividualVolume( parameterValue );
            else if( parameterName == "largestindividualvolume" ) SetLargestIndividualVolume( parameterValue );
            else if( parameterName == "preferredpreyvolumeratio" ) SetPreferredPreyVolumeRatio( parameterValue );
            else if( parameterName == "preferencefunctionwidth" ) SetPreferenceFunctionWidth( parameterValue );
            else if( parameterName == "sizeclasssubsetfraction" ) SetSizeClassSubsetFraction( parameterValue );
            else if( parameterName == "halfsaturationconstant" ) SetHalfSaturationConstantFraction( parameterValue );

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

    mSmallestVolumeExponent = Maths::Get( )->Log10( mSmallestIndividualVolume );
    mLargestVolumeExponent = Maths::Get( )->Log10( mLargestIndividualVolume );

    double sizeClassExponentIncrement = ( mLargestVolumeExponent - mSmallestVolumeExponent ) / mNumberOfSizeClasses;

    for( unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex ) {
        double sizeClassMidPointExponent = mSmallestVolumeExponent + ( ( sizeClassIndex + 0.5 ) * sizeClassExponentIncrement );
        double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( sizeClassIndex * sizeClassExponentIncrement );

        mSizeClassBoundaries[ sizeClassIndex ] = Maths::Get( )->Pow( 10, sizeClassBoundaryExponent );
        mSizeClassMidPoints[ sizeClassIndex ] = Maths::Get( )->Pow( 10, sizeClassMidPointExponent );
    }

    double sizeClassBoundaryExponent = mSmallestVolumeExponent + ( mNumberOfSizeClasses * sizeClassExponentIncrement );

    mSizeClassBoundaries[ mNumberOfSizeClasses ] = Maths::Get( )->Pow( 10, sizeClassBoundaryExponent );

    mInterSizeClassPreferenceMatrix.resize( mNumberOfSizeClasses );
    mInterSizeClassVolumeMatrix.resize( mNumberOfSizeClasses );

    Types::HeterotrophProcessorPointer temporaryHeterotrophProcessor = new HeterotrophProcessor( );

    mPhytoplanktonSizeClassIndex = temporaryHeterotrophProcessor->FindSizeClassIndexFromVolume( mSmallestIndividualVolume );

    mTotalVolume = mInitialNutrientVolume + mInitialAutotrophVolume + mInitialHeterotrophVolume;
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

unsigned Parameters::GetRunTimeInSeconds( ) const {
    return mRunTimeInSeconds;
}

unsigned Parameters::GetRandomSeed( ) const {
    return mRandomSeed;
}

unsigned Parameters::GetSamplingRate( ) const {
    return mSamplingRate;
}

unsigned Parameters::GetNumberOfSizeClasses( ) const {
    return mNumberOfSizeClasses;
}

bool Parameters::GetInitialisationMethod( ) const {
    return mInitialisationMethod;
}

bool Parameters::GetApplyStarvationFunction( ) const {
    return mApplyStarvationFunction;
}

bool Parameters::GetWriteModelState( ) const {
    return mWriteModelState;
}

double Parameters::GetInitialNutrientVolume( ) const {
    return mInitialNutrientVolume;
}

double Parameters::GetInitialAutotrophVolume( ) const {
    return mInitialAutotrophVolume;
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

unsigned Parameters::GetPreferredPreyVolumeRatio( ) const {
    return mPreferredPreyVolumeRatio;
}

double Parameters::GetPreferenceFunctionWidth( ) const {
    return mPreferenceFunctionWidth;
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

double Parameters::GetFractionalMetabolicExpense( ) const {
    return mFractionalMetabolicExpense;
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

unsigned Parameters::GetPhytoplanktonSizeClassIndex( ) const {
    return mPhytoplanktonSizeClassIndex;
}

double Parameters::GetSmallestVolumeExponent( ) const {
    return mSmallestVolumeExponent;
}

double Parameters::GetLargestVolumeExponent( ) const {
    return mLargestVolumeExponent;
}

double Parameters::GetSizeClassBoundary( const unsigned index ) const {
    return mSizeClassBoundaries[ index ];
}

double Parameters::GetSizeClassMidPoint( const unsigned index ) const {
    return mSizeClassMidPoints[ index ];
}

const Types::FloatVector Parameters::GetSizeClassBoundaries( ) const {
    return mSizeClassBoundaries;
}

const Types::FloatVector Parameters::GetSizeClassMidPoints( ) const {
    return mSizeClassMidPoints;
}

double Parameters::GetInterSizeClassPreference( const unsigned subjectIndex, const unsigned referenceIndex ) const {
    return mInterSizeClassPreferenceMatrix[ subjectIndex ][ referenceIndex ];
}

double Parameters::GetInterSizeClassVolume( const unsigned subjectIndex, const unsigned referenceIndex ) const {
    return mInterSizeClassVolumeMatrix[ subjectIndex ][ referenceIndex ];
}

double Parameters::GetTotalVolume( ) const {
    return mTotalVolume;
}

double Parameters::GetHalfSaturationConstant( ) const {
    return mHalfSaturationConstant;
}

const Types::FloatVector Parameters::GetInterSizeClassPreferenceVector( const unsigned index ) const {
    return mInterSizeClassPreferenceMatrix[ index ];
}

const Types::FloatVector Parameters::GetInterSizeClassVolumeVector( const unsigned index ) const {
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

void Parameters::SetInitialisationMethod( const bool initialisationMethod ) {
    mInitialisationMethod = initialisationMethod;
}

void Parameters::SetApplyStarvationFunction( const bool applyStarvationFunction ) {
    mApplyStarvationFunction = applyStarvationFunction;
}

void Parameters::SetWriteModelState( const bool writeModelState ) {
    mWriteModelState = writeModelState;
}

void Parameters::SetInitialNutrientVolume( const double initialNutrientVolume ) {
    mInitialNutrientVolume = initialNutrientVolume;
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
