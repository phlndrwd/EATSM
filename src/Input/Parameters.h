#ifndef PARAMETERS
#define PARAMETERS

#include "Types.h"
#include "Constants.h"

class Parameters {
public:
    ~Parameters( );

    static Types::ParametersPointer Get( );

    bool Initialise( const Types::StringMatrix& );

    void CalculateParameters( );

    // Getters
    unsigned GetRunTimeInSeconds( ) const;
    unsigned GetRandomSeed( ) const;
    unsigned GetSamplingRate( ) const;
    unsigned GetNumberOfSizeClasses( ) const;

    unsigned InitialisationMethod( ) const;

    double GetInitialNutrientVolume( ) const;
    double GetInitialAutotrophVolume( ) const;
    double GetInitialHeterotrophVolume( ) const;
    double GetMinimumHeterotrophVolume( ) const;

    double GetSmallestIndividualVolume( ) const;
    double GetLargestIndividualVolume( ) const;

    unsigned GetPreferredPreyVolumeRatio( ) const;
    double GetPreferenceFunctionWidth( ) const;

    double GetSizeClassSubsetFraction( ) const;
    double GetHalfSaturationConstantFraction( ) const;
    double GetAssimilationEfficiency( ) const;
    double GetFractionalMetabolicExpense( ) const;
    double GetMetabolicIndex( ) const;

    double GetMutationProbability( ) const;
    double GetMutationStandardDeviation( ) const;

    // Setters
    void SetRandomSeed( const unsigned );
    void SetRunTimeInSeconds( const unsigned );
    void SetSamplingRate( const unsigned );
    void SetNumberOfSizeClasses( const unsigned );

    void SetInitialisationMethod( const unsigned );

    void SetInitialNutrientVolume( const double );
    void SetInitialAutotrophVolume( const double );
    void SetInitialHeterotrophVolume( const double );
    void SetMinimumHeterotrophVolume( const double );

    void SetSmallestIndividualVolume( const double );
    void SetLargestIndividualVolume( const double );

    void SetPreferredPreyVolumeRatio( const unsigned );
    void SetPreferenceFunctionWidth( const double );

    void SetSizeClassSubsetFraction( const double );
    void SetHalfSaturationConstantFraction( const double );
    void SetAssimilationEfficiency( const double );
    void SetFractionalMetabolicExpense( const double );
    void SetMetabolicIndex( const double );

    void SetMutationProbability( const double );
    void SetMutationStandardDeviation( const double );

    // Calculated variables

    double GetSmallestVolumeExponent( ) const;
    double GetLargestVolumeExponent( ) const;

    unsigned GetPhytoplanktonSizeClassIndex( ) const;

    double GetSizeClassBoundary( const unsigned ) const;
    double GetSizeClassMidPoint( const unsigned ) const;

    double GetInterSizeClassPreference( const unsigned, const unsigned ) const;
    double GetInterSizeClassVolume( const unsigned, const unsigned ) const;

    double GetTotalVolume( ) const;
    double GetHalfSaturationConstant( ) const;

    const Types::FloatVector GetSizeClassBoundaries( ) const;
    const Types::FloatVector GetSizeClassMidPoints( ) const;

    const Types::FloatVector GetInterSizeClassPreferenceVector( const unsigned ) const;
    const Types::FloatVector GetInterSizeClassVolumeVector( const unsigned ) const;

private:
    Parameters( );

    static Types::ParametersPointer mThis;

    // User defined constants
    unsigned mRandomSeed;
    unsigned mRunTimeInSeconds;
    unsigned mSamplingRate;
    unsigned mNumberOfSizeClasses;

    unsigned mInitialisationMethod;

    double mInitialNutrientVolume;
    double mInitialAutotrophVolume;
    double mInitialHeterotrophVolume;
    double mMinimumHeterotrophVolume;

    double mSmallestIndividualVolume;
    double mLargestIndividualVolume;
    double mSizeClassSubsetFraction;
    double mHalfSaturationConstantFraction;

    unsigned mPreferredPreyVolumeRatio;
    double mPreferenceFunctionWidth;

    double mAssimilationEfficiency;
    double mFractionalMetabolicExpense;
    double mMetabolicIndex;

    double mMutationProbability;
    double mMutationStandardDeviation;

    // Calculated variables

    unsigned mPhytoplanktonSizeClassIndex;

    double mSmallestVolumeExponent;
    double mLargestVolumeExponent;

    double mTotalVolume;
    double mHalfSaturationConstant;

    Types::FloatVector mSizeClassBoundaries;
    Types::FloatVector mSizeClassMidPoints;

    Types::FloatMatrix mInterSizeClassPreferenceMatrix;
    Types::FloatMatrix mInterSizeClassVolumeMatrix;
};

#endif
