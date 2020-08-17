#include "Heterotrophs.h"

#include "HeterotrophProcessor.h"
#include "HeterotrophData.h"
#include "Nutrient.h"
#include "Autotrophs.h"
#include "Parameters.h"
#include "Individual.h"
#include "InitialState.h"
#include "HeritableTraits.h"
#include "DataRecorder.h"
#include "RandomSimple.h"

#include <iostream>
#include <omp.h>

Heterotrophs::Heterotrophs( ) {
}

Heterotrophs::Heterotrophs( Types::NutrientPointer nutrient, Types::AutotrophsPointer autotrophs ) {
    mNutrient = nutrient;
    mAutotrophs = autotrophs;
    mHeterotrophProcessor = new HeterotrophProcessor( );
    mHeterotrophData = new HeterotrophData( );

    CreateInitialPopulation( );
}

Heterotrophs::~Heterotrophs( ) {
    delete mHeterotrophProcessor;
    delete mHeterotrophData;

    delete mAutotrophs;
    delete mNutrient;

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned int individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            delete mLivingMatrix[ sizeClassIndex ][ individualIndex ];
        }
        mLivingMatrix[ sizeClassIndex ].clear( );
    }
}

void Heterotrophs::CreateInitialPopulation( ) {
    mLivingMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mDeadMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    if( InitialState::Get( )->IsInitialised( ) == false ) {
        unsigned initialPopulationSize = 0;
        double secondaryProducerVolume = Parameters::Get( )->GetSmallestIndividualVolume( ) * Parameters::Get( )->GetPreferredPreyVolumeRatio( );

        unsigned firstPopulatedIndex = mHeterotrophProcessor->FindSizeClassIndexFromVolume( secondaryProducerVolume );
        double individualVolume = Parameters::Get( )->GetSizeClassMidPoint( firstPopulatedIndex );
        double geneValue = mHeterotrophProcessor->VolumeToTraitValue( individualVolume );

        double initialHeterotrophVolume = Parameters::Get( )->GetInitialHeterotrophVolume( );

        while( individualVolume <= initialHeterotrophVolume ) {
            initialHeterotrophVolume -= individualVolume;
            Types::IndividualPointer individual = new Individual( individualVolume, geneValue, firstPopulatedIndex );
            mLivingMatrix[ firstPopulatedIndex ].push_back( individual );
            ++initialPopulationSize;
        }
        if( initialHeterotrophVolume > 0 )
            mNutrient->AddToVolume( initialHeterotrophVolume );

        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize << " individuals." << std::endl;
    } else {
        mLivingMatrix = InitialState::Get( )->GetHeterotrophs( );
        std::cout << "Heterotrophic size classes initialised with " << InitialState::Get( )->GetInitialPopulationSize( ) << " individuals." << std::endl;
    }
}

void Heterotrophs::Update( ) {
    // Feeding
    mTimer.Start( );
    Feeding( );
    mHeterotrophData->AddToTimeFeeding( mTimer.Stop( ) );
    // Metabolising
    mTimer.Start( );
    Metabolisation( );
    mHeterotrophData->AddToTimeMetabolising( mTimer.Stop( ) );
    // Reproducing
    mTimer.Start( );
    Reproduction( );
    mHeterotrophData->AddToTimeReproducing( mTimer.Stop( ) );
    // Starving
    mTimer.Start( );
    Starvation( );
    mHeterotrophData->AddToTimeStarving( mTimer.Stop( ) );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData->InitialiseDataStructures( );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );
        for( unsigned int individualIndex = 0; individualIndex < sizeClassPopulation; ++individualIndex ) {
            mHeterotrophData->AddIndividualData( mLivingMatrix[ sizeClassIndex ][ individualIndex ] );
        }
        double sizeClassMultiplier = 1 / ( double ) sizeClassPopulation;
        mHeterotrophData->AddSizeClassData( sizeClassIndex, sizeClassPopulation, sizeClassMultiplier );
    }
    mHeterotrophData->NormaliseData( );
    mHeterotrophData->RecordOutputData( );

    return mHeterotrophData->AreHeterotrophsAlive( );
}

unsigned Heterotrophs::GetSizeClassPopulation( const unsigned sizeClassIndex ) const {
    return mLivingMatrix[ sizeClassIndex ].size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned sizeClassIndex, const unsigned individualIndex ) const {
    return mLivingMatrix[ sizeClassIndex ][ individualIndex ];
}

void Heterotrophs::CalculateFeedingProbabilities( ) {
    for( unsigned predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {
        if( mLivingMatrix[ predatorIndex ].size( ) > 0 ) {
            unsigned coupledSizeClassIndex = 0;
            double effectivePreyVolume = 0;
            double highestEffectiveSizeClassVolume = 0;

            for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {
                double effectiveSizeClassVolume = 0;
                // Add the result of the autotroph volume - no frequency coefficient.
                if( preyIndex == Parameters::Get( )->GetAutotrophSizeClassIndex( ) )
                    effectiveSizeClassVolume = Parameters::Get( )->GetInterSizeClassPreference( predatorIndex, preyIndex ) * mAutotrophs->GetVolume( );
                // Add the result of the heterotrophs.
                if( preyIndex != predatorIndex ) effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * mLivingMatrix[ preyIndex ].size( );
                else effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * ( mLivingMatrix[ preyIndex ].size( ) - 1 );

                mHeterotrophData->SetEffectiveSizeClassVolume( predatorIndex, preyIndex, effectiveSizeClassVolume );

                if( effectiveSizeClassVolume > highestEffectiveSizeClassVolume ) {
                    highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
                    coupledSizeClassIndex = preyIndex;
                }
                effectivePreyVolume += effectiveSizeClassVolume;
            }
            mHeterotrophData->SetCoupledSizeClassIndex( predatorIndex, coupledSizeClassIndex );
            mHeterotrophData->SetEffectivePreyVolume( predatorIndex, effectivePreyVolume );
            mHeterotrophData->SetFeedingProbability( predatorIndex, mHeterotrophProcessor->CalculateFeedingProbability( predatorIndex, effectivePreyVolume ) );
        }
    }
}

void Heterotrophs::Feeding( ) {
    CalculateFeedingProbabilities( );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = mLivingMatrix[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassPopulationSubset = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset; ++potentialEncounterIndex ) {
                if( RandomSimple::Get( )->GetUniform( ) <= mHeterotrophData->GetFeedingProbability( sizeClassIndex ) ) {
                    Types::IndividualPointer predator = GetRandomIndividualFromSizeClass( sizeClassIndex );

                    unsigned coupledIndex = mHeterotrophData->GetCoupledSizeClassIndex( sizeClassIndex );

                    if( coupledIndex == Parameters::Get( )->GetAutotrophSizeClassIndex( ) )
                        FeedFromAutotrophs( predator );
                    else
                        FeedFromHeterotrophs( predator, coupledIndex );
                }
            }
        }
    }
    DeleteDead( );
}

void Heterotrophs::Metabolisation( ) {

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        for( unsigned individualIndex = 0; individualIndex < mLivingMatrix[ sizeClassIndex ].size( ); ++individualIndex ) {
            Types::IndividualPointer individual = mLivingMatrix[ sizeClassIndex ][ individualIndex ];
            double metabolicDeduction = mHeterotrophProcessor->CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                double waste = individual->Metabolise( metabolicDeduction );
                mNutrient->AddToVolume( waste );
                if( mHeterotrophProcessor->UpdateSizeClassIndex( individual ) == true )
                    MoveSizeClass( individual, sizeClassIndex );

            } else StarveToDeath( individual );
        }
    }
    DeleteDead( );
}

void Heterotrophs::Reproduction( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < mLivingMatrix[ sizeClassIndex ].size( ); ++individualIndex ) {
            Types::IndividualPointer potentialParent = mLivingMatrix[ sizeClassIndex ][ individualIndex ];

            if( potentialParent->GetVolumeActual( ) >= potentialParent->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = potentialParent->Reproduce( );

                if( mHeterotrophProcessor->UpdateSizeClassIndex( potentialParent ) == true )
                    MoveSizeClass( potentialParent, sizeClassIndex );

                childIndividual->SetSizeClassIndex( potentialParent->GetSizeClassIndex( ) );
                if( childIndividual->GetHeritableTraits( )->IsValueMutant( Constants::eVolume ) == true ) {
                    mHeterotrophProcessor->UpdateSizeClassIndex( childIndividual );
                    mHeterotrophData->IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), Constants::eVolume );
                }
                mHeterotrophData->IncrementBirthFrequencies( potentialParent->GetSizeClassIndex( ), childIndividual->GetSizeClassIndex( ) );

                mChildVector.push_back( childIndividual );
            }
        }
    }
    AddChildren( );
}

void Heterotrophs::Starvation( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = mLivingMatrix[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassSubsetSize = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation ) {
                Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                if( RandomSimple::Get( )->GetUniform( ) <= mHeterotrophProcessor->CalculateStarvationProbability( individual ) )
                    StarveToDeath( individual );
            }
        }
    }
    DeleteDead( );
}

void Heterotrophs::FeedFromAutotrophs( const Types::IndividualPointer grazer ) {
    double smallestIndividualVolume = Parameters::Get( )->GetSmallestIndividualVolume( );

    if( mAutotrophs->GetVolume( ) > smallestIndividualVolume ) {
        mAutotrophs->SubtractFromVolume( smallestIndividualVolume );
        mHeterotrophData->IncrementVegetarianFrequencies( grazer );

        double waste = grazer->ConsumePreyVolume( smallestIndividualVolume );
        double trophicLevel = grazer->GetTrophicLevel( );

        if( trophicLevel != 0 ) grazer->SetTrophicLevel( ( trophicLevel + 2 ) * 0.5 );
        else grazer->SetTrophicLevel( 2 );
        mNutrient->AddToVolume( waste );
    }
}

void Heterotrophs::FeedFromHeterotrophs( const Types::IndividualPointer predator, unsigned coupledIndex ) {
    Types::IndividualPointer prey = GetRandomIndividualFromSizeClass( coupledIndex, predator );

    double preyVolume = prey->GetVolumeActual( );
    mHeterotrophData->IncrementCarnivoreFrequencies( predator, prey );

    double waste = predator->ConsumePreyVolume( preyVolume );

    double predatorTrophicLevel = predator->GetTrophicLevel( );
    double preyTrophicLevel = prey->GetTrophicLevel( );

    double trophicLevel = -1;

    if( predatorTrophicLevel != 0 ) {
        if( preyTrophicLevel != 0 ) trophicLevel = ( predatorTrophicLevel + preyTrophicLevel + 1 ) * 0.5;
        else  trophicLevel = ( predatorTrophicLevel + 3 ) * 0.5;
    } else {
        if( preyTrophicLevel != 0 ) trophicLevel = preyTrophicLevel + 1;
        else trophicLevel = 3;
    }

    predator->SetTrophicLevel( trophicLevel );
    mNutrient->AddToVolume( waste );
    KillIndividual( prey );
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer individual ) const {
    // TODO - Improve this function. Risk of looping for a long time to randomly select one of few living individuals...
    unsigned sizeClassPopulation = mLivingMatrix[ sizeClassIndex ].size( );
    unsigned sizeClassLivingFrequency = sizeClassPopulation - mDeadMatrix[ sizeClassIndex ].size( );

    if( individual != NULL && individual->GetSizeClassIndex( ) == sizeClassIndex )
        sizeClassLivingFrequency -= 1;

    Types::IndividualPointer randomIndividual = NULL;

    if( sizeClassLivingFrequency > 0 ) {
        do {
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( sizeClassPopulation - 1 );
            randomIndividual = mLivingMatrix[ sizeClassIndex ][ randomIndividualIndex ];
        } while( randomIndividual->IsDead( ) == true || randomIndividual == individual );
    }
    return randomIndividual;
}

void Heterotrophs::DeleteDead( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < mDeadMatrix[ sizeClassIndex ].size( ); ++individualIndex )
            DeleteIndividual( mDeadMatrix[ sizeClassIndex ][ individualIndex ] );

        mDeadMatrix[ sizeClassIndex ].clear( );
    }
}

void Heterotrophs::DeleteIndividual( Types::IndividualPointer individual ) {
    RemoveFromSizeClass( individual, individual->GetSizeClassIndex( ) );
    delete individual;
}

void Heterotrophs::MoveSizeClass( const Types::IndividualPointer individual, const unsigned oldSizeClassIndex ) {
    RemoveFromSizeClass( individual, oldSizeClassIndex );
    mLivingMatrix[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::RemoveFromSizeClass( const Types::IndividualPointer individual, const unsigned sizeClassIndex ) {
    for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
        if( individual == mLivingMatrix[ sizeClassIndex ][ individualIndex ] ) {
            mLivingMatrix[ sizeClassIndex ].erase( mLivingMatrix[ sizeClassIndex ].begin( ) + individualIndex );
            break;
        }
    }
}

void Heterotrophs::StarveToDeath( Types::IndividualPointer individual ) {
    mNutrient->AddToVolume( individual->GetVolumeActual( ) );
    mHeterotrophData->IncrementStarvedFrequencies( individual->GetSizeClassIndex( ) );
    KillIndividual( individual );
}

void Heterotrophs::KillIndividual( Types::IndividualPointer individual ) {
    individual->Kill( );
    mDeadMatrix[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::AddChildren( ) {
    for( unsigned childIndex = 0; childIndex < mChildVector.size( ); ++childIndex ) {
        Types::IndividualPointer child = mChildVector[ childIndex ];
        mLivingMatrix[ child->GetSizeClassIndex( ) ].push_back( child );
    }
    mChildVector.clear( );
}