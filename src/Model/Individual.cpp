#include "Individual.h"

#include "HeritableTraits.h"
#include "Types.h"
#include "Parameters.h"
#include "RandomSFMT.h"
#include "TimeStep.h"
#include "HeterotrophProcessor.h"

// For model initialisation.
Individual::Individual( Types::HeterotrophProcessorPointer heterotrophProcessor, const double volumeHeritable, const unsigned sizeClassIndex ) {
    mVolumeHeritable = volumeHeritable;
    mSizeClassIndex = sizeClassIndex;

    Types::DoubleVector heritableTraitValues;
    heritableTraitValues.push_back( heterotrophProcessor->VolumeToGeneValue( mVolumeHeritable ) );
    heritableTraitValues.push_back( RandomSFMT::Get( )->GetUniform( ) );
    mHeritableTraits = new HeritableTraits( heritableTraitValues );

    mVolumeActual = mVolumeHeritable;
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mTrophicLevel = 0;
    mIsDead = false;
}

// For reproduction.
Individual::Individual( const Types::HeritableTraitsPointer heritableTraits, const double volumeHeritable, const double volumeActual, const double volumeMinimum, const double trophicLevel ) {
    mHeritableTraits = heritableTraits;
    mVolumeHeritable = volumeHeritable;
    mVolumeActual = volumeActual;
    mVolumeMinimum = volumeMinimum;
    mTrophicLevel = trophicLevel;

    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mIsDead = false;
}

// For model restart.
Individual::Individual( Types::HeterotrophProcessorPointer heterotrophProcessor, const double geneValue, const double volumeActual, const unsigned sizeClassIndex ) {
    Types::DoubleVector geneValues;
    geneValues.push_back( geneValue );
    mHeritableTraits = new HeritableTraits( geneValues );
    mVolumeActual = volumeActual;
    mSizeClassIndex = sizeClassIndex;

    mVolumeHeritable = heterotrophProcessor->GeneValueToVolume( mHeritableTraits->GetValue( Constants::eVolume ) );
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mTrophicLevel = 0;
    mIsDead = false;
}

Individual::~Individual( ) {
    delete mHeritableTraits;
}

Types::IndividualPointer Individual::Reproduce( Types::HeterotrophProcessorPointer heterotrophProcessor ) {
    Types::HeritableTraitsPointer childHeritableTraits = mHeritableTraits->GetChildTraits( );
    Types::IndividualPointer childIndividual = 0;

    double childVolumeHeritable = 0;
    double childVolumeActual = 0;
    double childVolumeMinimum = 0;

    if( childHeritableTraits->IsValueMutant( Constants::eVolume ) == false ) {
        childVolumeActual = mVolumeActual / 2;
        childVolumeHeritable = mVolumeHeritable;
        childVolumeMinimum = mVolumeMinimum;
    } else {
        childVolumeHeritable = heterotrophProcessor->GeneValueToVolume( childHeritableTraits->GetValue( Constants::eVolume ) );
        childVolumeMinimum = childVolumeHeritable * Constants::cMinimumFractionalVolume;

        if( childVolumeHeritable < mVolumeActual ) {
            childVolumeActual = childVolumeHeritable;
        } else {
            childVolumeActual = mVolumeActual / 2;
        }
    }
    mVolumeActual = mVolumeActual - childVolumeActual;
    childIndividual = new Individual( childHeritableTraits, childVolumeHeritable, childVolumeActual, childVolumeMinimum, mTrophicLevel );

    return childIndividual;
}

double Individual::ConsumePreyVolume( const double preyVolume ) {
    double volumeAssimilated = preyVolume * Parameters::Get( )->GetAssimilationEfficiency( );
    double wasteVolume = preyVolume - volumeAssimilated;

    mVolumeActual += volumeAssimilated;

    return wasteVolume;
}

double Individual::Metabolise( const double metabolicDeduction ) {
    ++mAge;
    mVolumeActual -= metabolicDeduction;
    return metabolicDeduction;
}

Types::HeritableTraitsPointer Individual::GetHeritableTraits( ) const {
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

void Individual::SetTrophicLevel( const double trophicLevel ) {
    mTrophicLevel = trophicLevel;
}

void Individual::SetSizeClassIndex( const unsigned sizeClassIndex ) {
    mSizeClassIndex = sizeClassIndex;
}

void Individual::SetAge( const unsigned age ) {
    mAge = age;
}

void Individual::Kill( ) {
    mIsDead = true;
}
