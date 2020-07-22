#ifndef PARAMETERS
#define PARAMETERS

#include "Types.h"
#include "Constants.h"

class Parameters {
public:
    ~Parameters( );

    static Types::ParametersPointer Get( );

    bool Initialise( const Types::StringMatrix& );

    // Getters
    unsigned& GetRunTimeInSeconds( );
    unsigned& GetRandomSeed( );
    unsigned& GetSamplingRate( );
    unsigned& GetNumberOfSizeClasses( );

    bool GetReadModelState( );
    bool GetWriteModelState( );

    double& GetInitialAutotrophVolume( );
    double& GetInitialHeterotrophVolume( );
    double& GetMinimumHeterotrophVolume( );

    double& GetSmallestIndividualVolume( );
    double& GetLargestIndividualVolume( );

    unsigned& GetPreferredPreyVolumeRatio( );
    double& GetPreferenceFunctionWidth( );

    double& GetSizeClassSubsetFraction( );
    double& GetHalfSaturationConstantFraction( );

    double& GetAssimilationEfficiency( );
    double& GetFractionalMetabolicExpense( );
    double& GetMetabolicIndex( );

    double& GetMutationProbability( );
    double& GetMutationStandardDeviation( );

    // Setters
    void SetRandomSeed( const unsigned );
    void SetRunTimeInSeconds( const unsigned );
    void SetSamplingRate( const unsigned );
    void SetNumberOfSizeClasses( const unsigned );

    void SetReadModelState( const bool );
    void SetWriteModelState( const bool );

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

    double& GetSmallestVolumeExponent( );
    double& GetLargestVolumeExponent( );

    unsigned& GetAutotrophSizeClassIndex( );

    double GetSizeClassBoundary( const unsigned ) const;
    double GetSizeClassMidPoint( const unsigned ) const;

    double GetInterSizeClassPreference( const unsigned, const unsigned ) const;
    double GetInterSizeClassVolume( const unsigned, const unsigned ) const;

    double& GetTotalVolume( );
    double& GetHalfSaturationConstant( );

    const Types::FloatVector& GetSizeClassBoundaries( );
    const Types::FloatVector& GetSizeClassMidPoints( );

    const Types::FloatVector& GetInterSizeClassPreferenceVector( const unsigned ) const;
    const Types::FloatVector& GetInterSizeClassVolumeVector( const unsigned ) const;

private:
    Parameters( );
    void CalculateParameters( );

    static Types::ParametersPointer mThis;

    // User defined constants
    unsigned mRandomSeed;
    unsigned mRunTimeInSeconds;
    unsigned mSamplingRate;
    unsigned mNumberOfSizeClasses;

    bool mReadModelState;
    bool mWriteModelState;

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
    unsigned mAutotrophSizeClassIndex;

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
