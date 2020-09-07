#include "Individual.h"

#include "HeritableTraits.h"
#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "TimeStep.h"

// For model initialisation.
Individual::Individual( const HeritableTraits& heritableTraits, const double volumeHeritable, const unsigned sizeClassIndex ):
mHeritableTraits( heritableTraits.GetValues( ), heritableTraits.AreTraitsMutant( ) ) {
    mVolumeHeritable = volumeHeritable;
    mSizeClassIndex = sizeClassIndex;

    mVolumeActual = mVolumeHeritable;
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mStarvationMultiplier = 1 / ( mVolumeHeritable - mVolumeMinimum );

    mAge = 0;
    mTrophicLevel = 0;
    mHasFed = false;
    mIsDead = false;
}

// For reproduction.
Individual::Individual( const HeritableTraits& heritableTraits, const double volumeHeritable, const double volumeActual, const double volumeMinimum, const double trophicLevel, const unsigned sizeClassIndex ):
mHeritableTraits( heritableTraits.GetValues( ), heritableTraits.AreTraitsMutant( ) ) {
    mVolumeHeritable = volumeHeritable;
    mVolumeActual = volumeActual;
    mVolumeMinimum = volumeMinimum;
    mTrophicLevel = trophicLevel;
    mSizeClassIndex = sizeClassIndex;

    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mStarvationMultiplier = 1 / ( mVolumeHeritable - mVolumeMinimum );

    mAge = 0;
    mHasFed = false;
    mIsDead = false;
}

// For model restart.
Individual::Individual( const HeritableTraits& heritableTraits, const double volumeHeritable, const double volumeActual, const unsigned sizeClassIndex ):
mHeritableTraits( heritableTraits.GetValues( ), heritableTraits.AreTraitsMutant( ) ) {
    mVolumeActual = volumeActual;
    mSizeClassIndex = sizeClassIndex;

    mVolumeHeritable = volumeHeritable;
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mStarvationMultiplier = 1 / ( mVolumeHeritable - mVolumeMinimum );

    mAge = 0;
    mTrophicLevel = 0;
    mHasFed = false;
    mIsDead = false;
}

Individual::Individual( const HeritableTraits& heritableTraits, const double volumeHeritable, const double volumeMinimum, const double volumeReproduction, const double volumeActual, const double trophicLevel, const double starvationMultiplier, const unsigned sizeClassIndex, const unsigned age, const bool hasFed, const bool isDead ):
mHeritableTraits( heritableTraits.GetValues( ), heritableTraits.AreTraitsMutant( ) ), mVolumeHeritable( std::move( volumeHeritable ) ), mVolumeMinimum( std::move( volumeMinimum ) ), mVolumeReproduction( std::move( volumeReproduction ) ), mVolumeActual( std::move( volumeActual ) ),
mTrophicLevel( std::move( trophicLevel ) ), mStarvationMultiplier( std::move( starvationMultiplier ) ), mSizeClassIndex( std::move( sizeClassIndex ) ), mAge( std::move( age ) ), mHasFed( std::move( hasFed ) ), mIsDead( std::move( isDead ) ) {

}

// Copy constructor
Individual::Individual( const Individual& individual ):
mHeritableTraits( individual.mHeritableTraits.GetValues( ), individual.mHeritableTraits.AreTraitsMutant( ) ) {
    mVolumeHeritable = individual.mVolumeHeritable;
    mVolumeMinimum = individual.mVolumeMinimum;
    mVolumeReproduction = individual.mVolumeReproduction;

    mVolumeActual = individual.mVolumeActual;
    mTrophicLevel = individual.mTrophicLevel;

    mStarvationMultiplier = individual.mStarvationMultiplier;

    mSizeClassIndex = individual.mSizeClassIndex;
    mAge = individual.mAge;

    mHasFed = individual.mHasFed;
    mIsDead = individual.mIsDead;
}

// Move constructor

Individual::Individual( const Individual&& individual ) noexcept:
mHeritableTraits( individual.mHeritableTraits.GetValues( ), individual.mHeritableTraits.AreTraitsMutant( ) ) {
    mVolumeHeritable = std::move( individual.mVolumeHeritable );
    mVolumeMinimum = std::move( individual.mVolumeMinimum );
    mVolumeReproduction = std::move( individual.mVolumeReproduction );

    mVolumeActual = std::move( individual.mVolumeActual );
    mTrophicLevel = std::move( individual.mTrophicLevel );

    mStarvationMultiplier = std::move( individual.mStarvationMultiplier );

    mSizeClassIndex = std::move( individual.mSizeClassIndex );
    mAge = std::move( individual.mAge );

    mHasFed = std::move( individual.mHasFed );
    mIsDead = std::move( individual.mIsDead );
}

Individual::~Individual( ) {

}

Types::IndividualPointer Individual::Reproduce( ) {
    HeritableTraits childHeritableTraits = mHeritableTraits.GetChildTraits( );
    Types::IndividualPointer childIndividual = 0;

    double childVolumeHeritable = 0;
    double childVolumeActual = 0;
    double childVolumeMinimum = 0;

    if( childHeritableTraits.IsTraitMutant( Constants::eVolume ) == false ) {
        childVolumeActual = mVolumeActual * Constants::cReproductionMultiplier;
        childVolumeHeritable = mVolumeHeritable;
        childVolumeMinimum = mVolumeMinimum;
    } else {
        childVolumeHeritable = HeterotrophProcessor::TraitValueToVolume( childHeritableTraits.GetValue( Constants::eVolume ) );
        childVolumeMinimum = childVolumeHeritable * Constants::cMinimumFractionalVolume;

        if( childVolumeHeritable < mVolumeActual ) childVolumeActual = childVolumeHeritable;
        else childVolumeActual = mVolumeActual * Constants::cReproductionMultiplier;
    }
    mVolumeActual = mVolumeActual - childVolumeActual;
    // Construct with HeritableTraits, not values alone... no mutants recorded
    childIndividual = new Individual( childHeritableTraits, childVolumeHeritable, childVolumeActual, childVolumeMinimum, mTrophicLevel, mSizeClassIndex );

    return childIndividual;
}

Individual& Individual::operator = ( const Individual& individual ) {
    if( this != &individual ) {
        mHeritableTraits = individual.mHeritableTraits;

        mVolumeHeritable = individual.mVolumeHeritable;
        mVolumeMinimum = individual.mVolumeMinimum;
        mVolumeReproduction = individual.mVolumeReproduction;

        mVolumeActual = individual.mVolumeActual;
        mTrophicLevel = individual.mTrophicLevel;

        mStarvationMultiplier = individual.mStarvationMultiplier;

        mSizeClassIndex = individual.mSizeClassIndex;
        mAge = individual.mAge;

        mHasFed = individual.mHasFed;
        mIsDead = individual.mIsDead;
    }
    return *this;
}

bool Individual::operator == ( const Individual& individual ) {
    return ( mVolumeActual == individual.mVolumeActual &&
            mVolumeHeritable == individual.mVolumeHeritable );
}

double Individual::ConsumePreyVolume( const double preyVolume ) {
    double volumeAssimilated = preyVolume * Parameters::Get( )->GetAssimilationEfficiency( );
    double wasteVolume = preyVolume - volumeAssimilated;

    mVolumeActual += volumeAssimilated;
    mHasFed = true;

    return wasteVolume;
}

double Individual::Metabolise( const double metabolicDeduction ) {
    ++mAge;
    mVolumeActual -= metabolicDeduction;
    return metabolicDeduction;
}

HeritableTraits& Individual::GetHeritableTraits( ) {
    return mHeritableTraits;
}

double Individual::GetTrophicLevel( ) const {
    return mTrophicLevel;
}

unsigned Individual::GetSizeClassIndex( ) const {
    return mSizeClassIndex;
}

unsigned Individual::GetAge( ) const {
    return mAge;
}

bool Individual::HasFed( ) const {
    return mHasFed;
}

bool Individual::IsDead( ) const {
    return mIsDead;
}

double Individual::GetVolumeActual( ) const {
    return mVolumeActual;
}

double Individual::GetVolumeHeritable( ) const {
    return mVolumeHeritable;
}

double Individual::GetVolumeMinimum( ) const {
    return mVolumeMinimum;
}

double Individual::GetVolumeReproduction( ) const {
    return mVolumeReproduction;
}

double Individual::GetStarvationMultiplier( ) const {
    return mStarvationMultiplier;
}

void Individual::SetTrophicLevel( const double trophicLevel ) {
    mTrophicLevel = trophicLevel;
}

void Individual::SetSizeClassIndex( const unsigned sizeClassIndex ) {
    mSizeClassIndex = sizeClassIndex;
}

void Individual::SetAge( const unsigned age ) {
    mAge = age;
}

void Individual::SetHasFed( const bool hasFed ) {
    mHasFed = hasFed;
}

void Individual::Kill( ) {
    mIsDead = true;
}
