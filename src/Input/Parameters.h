#ifndef PARAMETERS
#define PARAMETERS

#include "Types.h"
#include "Constants.h"

class Parameters {
public:
    ~Parameters( );

    static Types::ParametersPointer Get( );

    void SetConstants( );

    void CalculateSizeClassVectors( );
    void CalculateVariables( );

    double GetParameterValueFromIndex( const unsigned int );

    // Getters

    const std::string GetExperimentName( ) const;

    unsigned int GetRunTimeInSeconds( ) const;
    unsigned int GetRandomSeed( ) const;
    unsigned int GetSamplingRate( ) const;
    unsigned int GetNumberOfSizeClasses( ) const;

    unsigned int GetHeterotrophInitialisationMethod( ) const;
    unsigned int GetPreferenceFunctionType( ) const;
    unsigned int GetFunctionalResponseType( ) const;
    unsigned int GetStarvationFunctionType( ) const;

    double GetInitialNutrientVolume( ) const;
    double GetInitialPhytoplanktonVolume( ) const;
    double GetInitialHeterotrophVolume( ) const;
    double GetMinimumHeterotrophVolume( ) const;

    double GetSmallestIndividualVolume( ) const;
    double GetLargestIndividualVolume( ) const;

    unsigned int GetPreferredPreyVolumeRatio( ) const;
    double GetPreferenceFunctionWidth( ) const;
    double GetPreferenceFunctionHeight( ) const;

    double GetSizeClassSubsetFraction( ) const;
    double GetHalfSaturationConstantFraction( ) const;
    double GetAssimilationEfficiency( ) const;
    double GetFractionalMetabolicExpensePerTimeStep( ) const;
    double GetMetabolicIndex( ) const;

    double GetMutationProbability( ) const;
    double GetMutationStandardDeviation( ) const;

    // Setters

    void SetExperimentName( const std::string );
    void SetRandomSeed( const unsigned int );
    void SetRunTimeInSeconds( const unsigned int );
    void SetSamplingRate( const unsigned int );
    void SetNumberOfSizeClasses( const unsigned int );

    void SetHeterotrophInitialisationMethod( const unsigned int );
    void SetPreferenceFunctionType( const unsigned int );
    void SetFunctionalResponseType( const unsigned int );
    void SetStarvationFunctionType( const unsigned int );

    void SetInitialNutrientVolume( const double );
    void SetInitialPhytoplanktonVolume( const double );
    void SetInitialHeterotrophVolume( const double );
    void SetMinimumHeterotrophVolume( const double );

    void SetSmallestIndividualVolume( const double );
    void SetLargestIndividualVolume( const double );

    void SetPreferredPreyVolumeRatio( const unsigned int );
    void SetPreferenceFunctionWidth( const double );
    void SetPreferenceFunctionHeight( const double );

    void SetSizeClassSubsetFraction( const double );
    void SetHalfSaturationConstantFraction( const double );
    void SetAssimilationEfficiency( const double );
    void SetFractionalMetabolicExpensePerTimeStep( const double );
    void SetMetabolicIndex( const double );

    void SetMutationProbability( const double );
    void SetMutationStandardDeviation( const double );

    // Calculated variables

    double GetSmallestVolumeExponent( ) const;
    double GetLargestVolumeExponent( ) const;

    unsigned int GetPhytoplanktonSizeClassIndex( ) const;

    double GetSizeClassBoundary( const unsigned int ) const;
    double GetSizeClassMidPoint( const unsigned int ) const;

    double GetInterSizeClassPreference( const unsigned int, const unsigned int ) const;
    double GetInterSizeClassVolume( const unsigned int, const unsigned int ) const;

    double GetTotalVolume( ) const;
    double GetHalfSaturationConstant( ) const;

    const Types::FloatVector GetSizeClassBoundaries( ) const;
    const Types::FloatVector GetSizeClassMidPoints( ) const;

    const Types::FloatVector GetInterSizeClassPreferenceVector( const unsigned int ) const;
    const Types::FloatVector GetInterSizeClassVolumeVector( const unsigned int ) const;

private:
    Parameters( );

    static Types::ParametersPointer mThis;

    // User defined constants

    std::string mExperimentName;
    unsigned int mRandomSeed;
    unsigned int mRunTimeInSeconds;
    unsigned int mSamplingRate;
    unsigned int mNumberOfSizeClasses;

    unsigned int mHeterotrophInitialisationMethod;
    unsigned int mPreferenceFunctionType;
    unsigned int mFunctionalResponseType;
    unsigned int mStarvationFunctionType;

    double mInitialNutrientVolume;
    double mInitialPhytoplanktonVolume;
    double mInitialHeterotrophVolume;
    double mMinimumHeterotrophVolume;

    double mSmallestIndividualVolume;
    double mLargestIndividualVolume;
    double mSizeClassSubsetFraction;
    double mHalfSaturationConstantFraction;

    unsigned int mPreferredPreyVolumeRatio;
    double mPreferenceFunctionWidth;
    double mPreferenceFunctionHeight;

    double mAssimilationEfficiency;
    double mFractionalMetabolicExpensePerTimeStep;
    double mMetabolicIndex;

    double mMutationProbability;
    double mMutationStandardDeviation;

    // Calculated variables

    unsigned int mPhytoplanktonSizeClassIndex;

    double mSmallestVolumeExponent;
    double mLargestVolumeExponent;

    double mTotalVolume;
    double mHalfSaturationConstant;

    Types::FloatVector mParameterValuesVector;

    Types::FloatVector mSizeClassBoundaries;
    Types::FloatVector mSizeClassMidPoints;

    Types::FloatMatrix mInterSizeClassPreferenceMatrix;
    Types::FloatMatrix mInterSizeClassVolumeMatrix;
};

#endif
