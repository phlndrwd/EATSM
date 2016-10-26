#include "Individual.h"

#include "Genome.h"
#include "Types.h"
#include "Maths.h"
#include "Parameters.h"
#include "Convertor.h"
#include "Logger.h"
#include "RandomSFMT.h"
#include "DataTag.h"
#include "Time.h"

/*
 * Construction and destruction
 */

// For model initialisation.

Individual::Individual( const double volumeHeritable, const unsigned sizeClassIndex ) {

    mVolumeHeritable = volumeHeritable;
    mSizeClassIndex = sizeClassIndex;

    // Initialise genome
    Types::DoubleVector genomeValues;
    genomeValues.push_back( Convertor::Get( )->VolumeToGeneValue( mVolumeHeritable ) );
    genomeValues.push_back( RandomSFMT::Get( )->GetUniform( ) );
    mGenome = new Genome( genomeValues );

    mVolumeActual = mVolumeHeritable;
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mTrophicLevel = 0;
    mIsDead = false;
    mTag = NULL;
}

// For reproduction.

Individual::Individual( const Types::GenomePointer genome, const double volumeHeritable, const double volumeActual, const double volumeMinimum, const double trophicLevel ) {

    mGenome = genome;
    mVolumeHeritable = volumeHeritable;
    mVolumeActual = volumeActual;
    mVolumeMinimum = volumeMinimum;
    mTrophicLevel = trophicLevel;

    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mIsDead = false;
    mTag = NULL;
}

// For model restart.

Individual::Individual( const double geneValue, const double volumeActual, const unsigned sizeClassIndex ) {

    Types::DoubleVector geneValues;
    geneValues.push_back( geneValue );
    mGenome = new Genome( geneValues );
    mVolumeActual = volumeActual;
    mSizeClassIndex = sizeClassIndex;

    mVolumeHeritable = Convertor::Get( )->GeneValueToVolume( mGenome->GetGeneValue( Constants::eVolumeGene ) );
    mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
    mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

    mAge = 0;
    mTrophicLevel = 0;
    mIsDead = false;
    mTag = NULL;
}

Individual::~Individual( ) {
    delete mGenome;
}

Types::IndividualPointer Individual::Reproduce( ) {

    Types::GenomePointer childGenome = mGenome->GetChildGenome( );
    Types::IndividualPointer childIndividual = 0;

    double childVolumeHeritable = 0;
    double childVolumeActual = 0;
    double childVolumeMinimum = 0;

    if( childGenome->IsMutantGeneValue( Constants::eVolumeGene ) == false ) {
        childVolumeActual = mVolumeActual / 2;
        childVolumeHeritable = mVolumeHeritable;
        childVolumeMinimum = mVolumeMinimum;
    } else {
        childVolumeHeritable = Convertor::Get( )->GeneValueToVolume( childGenome->GetGeneValue( Constants::eVolumeGene ) );
        childVolumeMinimum = childVolumeHeritable * Constants::cMinimumFractionalVolume;

        if( childVolumeHeritable < mVolumeActual ) {
            childVolumeActual = childVolumeHeritable;
        } else {
            childVolumeActual = mVolumeActual / 2;
        }
    }

    mVolumeActual = mVolumeActual - childVolumeActual;

    childIndividual = new Individual( childGenome, childVolumeHeritable, childVolumeActual, childVolumeMinimum, mTrophicLevel );

    return childIndividual;
}

double Individual::ConsumePreyVolume( const double preyVolume, const bool isHerbivory ) {
    double volumeAssimilated = preyVolume * Parameters::Get( )->GetAssimilationEfficiency( );
    double wasteVolume = preyVolume - volumeAssimilated;
    
    mVolumeActual += volumeAssimilated;
    
    if( mTag != NULL ) mTag->AddConsumptionEvent( preyVolume, volumeAssimilated, wasteVolume, isHerbivory );

    return wasteVolume;
}

double Individual::Metabolise( const double metabolicDeduction ) {

    ++mAge;

    mVolumeActual -= metabolicDeduction;

    return metabolicDeduction;
}

/*
 * Getters
 */

Types::GenomePointer Individual::GetGenome( ) const {
    return mGenome;
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

double* Individual::GetVolumeActualPointer( ) const {
    &mVolumeActual;
}

double* Individual::GetTrophicLevelPointer( ) const {
    &mTrophicLevel;
}

/*
 * Setters
 */

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

void Individual::SetTag( Types::DataTagPointer tag ) {
    mTag = tag;
}

void Individual::RecordTagData( ) {
    if( mTag != NULL ) mTag->RecordTimeSeriesData( );
}
