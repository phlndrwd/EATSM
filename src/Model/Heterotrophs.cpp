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
            delete mIndividualsLiving[ sizeClassIndex ][ individualIndex ];
        }
        mIndividualsLiving[ sizeClassIndex ].clear( );
    }
    mIndividualsLiving.clear( );
}

void Heterotrophs::CreateInitialPopulation( ) {
    mFedCount.resize( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );
    mIndividualsLiving.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mIndividualsDead.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

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
            mIndividualsLiving[ firstPopulatedIndex ].push_back( individual );
            ++initialPopulationSize;
        }
        if( initialHeterotrophVolume > 0 )
            mNutrient->AddToVolume( initialHeterotrophVolume );

        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize + 1 << " individuals." << std::endl;
    } else {
        mIndividualsLiving = InitialState::Get( )->GetHeterotrophs( );
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
    // Starving
    mTimer.Start( );
    Starvation( );
    mHeterotrophData->AddToTimeStarving( mTimer.Stop( ) );
    // Reproducing
    mTimer.Start( );
    Reproduction( );
    mHeterotrophData->AddToTimeReproducing( mTimer.Stop( ) );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData->InitialiseDataStructures( );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );
        for( unsigned int individualIndex = 0; individualIndex < sizeClassPopulation; ++individualIndex ) {
            mHeterotrophData->AddIndividualData( mIndividualsLiving[ sizeClassIndex ][ individualIndex ] );
        }
        double sizeClassMultiplier = 1 / ( double ) sizeClassPopulation;
        mHeterotrophData->AddSizeClassData( sizeClassIndex, sizeClassPopulation, sizeClassMultiplier );
    }
    mHeterotrophData->NormaliseData( );
    mHeterotrophData->RecordOutputData( );

    return mHeterotrophData->AreHeterotrophsAlive( );
}

unsigned Heterotrophs::GetSizeClassPopulation( const unsigned sizeClassIndex ) const {
    return mIndividualsLiving[ sizeClassIndex ].size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned sizeClassIndex, const unsigned individualIndex ) const {
    return mIndividualsLiving[ sizeClassIndex ][ individualIndex ];
}

void Heterotrophs::CalculateFeedingProbabilities( ) {
    for( unsigned predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {
        mFedCount[ predatorIndex ] = 0;
        if( mIndividualsLiving[ predatorIndex ].size( ) > 0 ) {
            unsigned coupledSizeClassIndex = 0;
            double effectivePreyVolume = 0;
            double highestEffectiveSizeClassVolume = 0;

            for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {
                double effectiveSizeClassVolume = 0;
                // Add the result of the autotroph volume - no frequency coefficient.
                if( preyIndex == Parameters::Get( )->GetAutotrophSizeClassIndex( ) )
                    effectiveSizeClassVolume = Parameters::Get( )->GetInterSizeClassPreference( predatorIndex, preyIndex ) * mAutotrophs->GetVolume( );
                // Add the result of the heterotrophs.
                unsigned preySizeClassPopulation = mIndividualsLiving[ preyIndex ].size( );
                if( preySizeClassPopulation > 0 ) {
                    if( preyIndex != predatorIndex ) effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * preySizeClassPopulation;
                    else effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * ( preySizeClassPopulation - 1 );
                }
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
        unsigned sizeClassPopulation = mIndividualsLiving[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {

            unsigned sizeClassPopulationSubset = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );
            unsigned coupledIndex = mHeterotrophData->GetCoupledSizeClassIndex( sizeClassIndex );

            for( unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset; ++potentialEncounterIndex ) {
                if( RandomSimple::Get( )->GetUniform( ) <= mHeterotrophData->GetFeedingProbability( sizeClassIndex ) ) {

                    Types::IndividualPointer predator = GetRandomPredatorFromSizeClass( sizeClassIndex );
                    if( predator != NULL ) {
                        if( coupledIndex == Parameters::Get( )->GetAutotrophSizeClassIndex( ) )
                            FeedFromAutotrophs( predator );
                        else
                            FeedFromHeterotrophs( predator, coupledIndex );
                    }
                }
            }
        }
    }
    DeleteDead( );
}

void Heterotrophs::Metabolisation( ) {

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        for( unsigned individualIndex = 0; individualIndex < mIndividualsLiving[ sizeClassIndex ].size( ); ++individualIndex ) {
            Types::IndividualPointer individual = mIndividualsLiving[ sizeClassIndex ][ individualIndex ];
            double metabolicDeduction = mHeterotrophProcessor->CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                
                individual->SetHasFed( false ); // Reset for the next time step
                double waste = individual->Metabolise( metabolicDeduction );
                mNutrient->AddToVolume( waste );

                // Individuals can move up a size class from having consumed  a 
                // lot. They need to move after this function has completed to 
                // avoid handling them twice.
                if( mHeterotrophProcessor->UpdateSizeClassIndex( individual ) == true )
                    StageForMoving( individual, sizeClassIndex );
                
            } else StarveToDeath( individual );
        }
    }
    MoveIndividuals( );
    DeleteDead( );
}

void Heterotrophs::Reproduction( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < mIndividualsLiving[ sizeClassIndex ].size( ); ++individualIndex ) {
            Types::IndividualPointer potentialParent = mIndividualsLiving[ sizeClassIndex ][ individualIndex ];

            if( potentialParent->GetVolumeActual( ) >= potentialParent->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = potentialParent->Reproduce( );

                // Parent data needs to be collected before size class is updated.
                mHeterotrophData->IncrementParentFrequencies( potentialParent->GetSizeClassIndex( ) );
                if( mHeterotrophProcessor->UpdateSizeClassIndex( potentialParent ) == true )
                    StageForMoving( potentialParent, sizeClassIndex );

                if( childIndividual->GetHeritableTraits( )->IsValueMutant( Constants::eVolume ) == true ) {
                    mHeterotrophProcessor->UpdateSizeClassIndex( childIndividual ); // Mutants may inherit more than a fixed fraction of the parent's volume
                    mHeterotrophData->IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), Constants::eVolume );
                }

                mHeterotrophData->IncrementChildFrequencies( childIndividual->GetSizeClassIndex( ) );
                mChildren.push_back( childIndividual );
            }
        }
    }
    MoveIndividuals( );
    AddChildren( );
}

void Heterotrophs::Starvation( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = mIndividualsLiving[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassSubsetSize = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation ) {
                Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                if( individual != NULL )
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
        ++mFedCount[ grazer->GetSizeClassIndex( )];

        double trophicLevel = grazer->GetTrophicLevel( );
        if( trophicLevel != 0 ) grazer->SetTrophicLevel( ( trophicLevel + 2 ) * 0.5 );
        else grazer->SetTrophicLevel( 2 );
        mNutrient->AddToVolume( waste );
    }
}

void Heterotrophs::FeedFromHeterotrophs( const Types::IndividualPointer predator, unsigned coupledIndex ) {
    Types::IndividualPointer prey = GetRandomPreyFromSizeClass( coupledIndex, predator );
    if( prey != NULL ) {
        double preyVolume = prey->GetVolumeActual( );
        mHeterotrophData->IncrementCarnivoreFrequencies( predator, prey );

        double waste = predator->ConsumePreyVolume( preyVolume );
        ++mFedCount[ predator->GetSizeClassIndex( ) ];

        double predatorTrophicLevel = predator->GetTrophicLevel( );
        double preyTrophicLevel = prey->GetTrophicLevel( );

        double trophicLevel = -1;

        if( predatorTrophicLevel != 0 ) {
            if( preyTrophicLevel != 0 ) trophicLevel = ( predatorTrophicLevel + preyTrophicLevel + 1 ) * 0.5;
            else trophicLevel = ( predatorTrophicLevel + 3 ) * 0.5;
        } else {
            if( preyTrophicLevel != 0 ) trophicLevel = preyTrophicLevel + 1;
            else trophicLevel = 3;
        }

        predator->SetTrophicLevel( trophicLevel );
        mNutrient->AddToVolume( waste );
        KillIndividual( prey );
    }
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex ) const {
    unsigned numberLiving = mIndividualsLiving[ sizeClassIndex ].size( );
    unsigned numberActive = numberLiving - mIndividualsDead[ sizeClassIndex ].size( );
    
    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        do {
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberLiving - 1 );
            randomIndividual = mIndividualsLiving[ sizeClassIndex ][ randomIndividualIndex ];
        } while( randomIndividual->IsDead( ) == true );
    }
    return randomIndividual;
}

Types::IndividualPointer Heterotrophs::GetRandomPredatorFromSizeClass( const unsigned sizeClassIndex ) const {
    unsigned numberLiving = mIndividualsLiving[ sizeClassIndex ].size( );
    unsigned numberActive = numberLiving - mIndividualsDead[ sizeClassIndex ].size( ) - mFedCount[ sizeClassIndex ];

    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        do {
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberLiving - 1 );
            randomIndividual = mIndividualsLiving[ sizeClassIndex ][ randomIndividualIndex ];
        } while( randomIndividual->IsDead( ) == true || randomIndividual->HasFed( ) );
    }
    return randomIndividual;
}

Types::IndividualPointer Heterotrophs::GetRandomPreyFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer predator ) const {
    unsigned numberOfLiving = mIndividualsLiving[ sizeClassIndex ].size( );
    unsigned numberActive = numberOfLiving - mIndividualsDead[ sizeClassIndex ].size( );

    // Only applicable when predator feeds from its own size class. Check 
    // ensures numberActive does not go out of bounds
    if( predator->GetSizeClassIndex( ) == sizeClassIndex && numberActive > 0 )
        --numberActive;
    
    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        do {
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberOfLiving - 1 );
            randomIndividual = mIndividualsLiving[ sizeClassIndex ][ randomIndividualIndex ];
        } while( randomIndividual->IsDead( ) == true || randomIndividual == predator );
    }
    return randomIndividual;
}

void Heterotrophs::StageForMoving( const Types::IndividualPointer individual, const unsigned oldSizeClassIndex ) {
    mOldSizeClassIndicies.push_back( oldSizeClassIndex );
    mIndividualsToMove.push_back( individual );
}

void Heterotrophs::MoveSizeClass( const Types::IndividualPointer individual, const unsigned oldSizeClassIndex ) {
    RemoveFromSizeClass( individual, oldSizeClassIndex );
    mIndividualsLiving[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::RemoveFromSizeClass( const Types::IndividualPointer individual, const unsigned sizeClassIndex ) {
    for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
        if( individual == mIndividualsLiving[ sizeClassIndex ][ individualIndex ] ) {
            mIndividualsLiving[ sizeClassIndex ].erase( mIndividualsLiving[ sizeClassIndex ].begin( ) + individualIndex );
            break;
        }
    }
}

void Heterotrophs::DeleteIndividual( Types::IndividualPointer individual ) {
    RemoveFromSizeClass( individual, individual->GetSizeClassIndex( ) );
    delete individual;
}

void Heterotrophs::StarveToDeath( Types::IndividualPointer individual ) {
    mNutrient->AddToVolume( individual->GetVolumeActual( ) );
    mHeterotrophData->IncrementStarvedFrequencies( individual->GetSizeClassIndex( ) );
    KillIndividual( individual );
}

void Heterotrophs::KillIndividual( Types::IndividualPointer individual ) {
    individual->Kill( );
    mIndividualsDead[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::AddChildren( ) {
    for( unsigned childIndex = 0; childIndex < mChildren.size( ); ++childIndex ) {
        mIndividualsLiving[ mChildren[ childIndex ]->GetSizeClassIndex( ) ].push_back( mChildren[ childIndex ] );
    }
    mChildren.clear( );
}

void Heterotrophs::MoveIndividuals( ) {
    for( unsigned index = 0; index < mIndividualsToMove.size( ); ++index ) {
        MoveSizeClass( mIndividualsToMove[ index ], mOldSizeClassIndicies[ index ] );
    }
    mOldSizeClassIndicies.clear( );
    mIndividualsToMove.clear( );
}

void Heterotrophs::DeleteDead( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < mIndividualsDead[ sizeClassIndex ].size( ); ++individualIndex )
            DeleteIndividual( mIndividualsDead[ sizeClassIndex ][ individualIndex ] );

        mIndividualsDead[ sizeClassIndex ].clear( );
    }
}