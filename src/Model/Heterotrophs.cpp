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

    for( unsigned individualIndex = 0; individualIndex < mLivingVector.size( ); ++individualIndex )
        delete mLivingVector[ individualIndex ];
}

void Heterotrophs::CreateInitialPopulation( ) {
    mLivingMatrix.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    mSizeClassDeadFrequencies.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
    
    if( Parameters::Get( )->GetReadModelState( ) == false ) {
        unsigned initialPopulationSize = 0;
        double secondaryProducerVolume = Parameters::Get( )->GetSmallestIndividualVolume( ) * Parameters::Get( )->GetPreferredPreyVolumeRatio( );
        unsigned firstPopulatedIndex = mHeterotrophProcessor->FindSizeClassIndexFromVolume( secondaryProducerVolume );
        double individualVolume = Parameters::Get( )->GetSizeClassMidPoint( firstPopulatedIndex );

        double initialHeterotrophVolume = Parameters::Get( )->GetInitialHeterotrophVolume( );

        while( individualVolume <= initialHeterotrophVolume ) {
            initialHeterotrophVolume -= individualVolume;
            Types::IndividualPointer individual = new Individual( mHeterotrophProcessor, individualVolume, firstPopulatedIndex );
            mLivingVector.push_back( individual );
            ++initialPopulationSize;
        }
        if( initialHeterotrophVolume > 0 )
            mNutrient->AddToVolume( initialHeterotrophVolume );

        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize << " individuals." << std::endl;
    } else {
        mLivingVector = InitialState::Get( )->GetHeterotrophs( );
        std::cout << "Heterotrophic size classes initialised with " << InitialState::Get( )->GetInitialPopulationSize( ) << " individuals." << std::endl;
    }
}

void Heterotrophs::ResetIndividualMatrix( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mLivingMatrix[ sizeClassIndex ].clear( );
    }
    //#pragma omp parallel num_threads(omp_get_num_procs()) shared(mLivingMatrix)
    {
        //#pragma omp for schedule(dynamic)
        for( unsigned individualIndex = 0; individualIndex < mLivingVector.size( ); ++individualIndex ) {
           Types::IndividualPointer individual = mLivingVector[ individualIndex ];
           mLivingMatrix[ individual->GetSizeClassIndex( ) ].push_back( individual );
        }
    }
}

void Heterotrophs::Update( ) {
    // Feeding
    mTimer.Start();
    Feeding( );
    mHeterotrophData->AddToTimeFeeding( mTimer.Stop() );
    // Metabolising
    mTimer.Start();
    Metabolisation( );
    mHeterotrophData->AddToTimeMetabolising( mTimer.Stop() );
    // Reproducing
    mTimer.Start();
    Reproduction( );
    mHeterotrophData->AddToTimeReproducing( mTimer.Stop() );
    // Starving
    mTimer.Start();
    Starvation( );
    mHeterotrophData->AddToTimeStarving( mTimer.Stop() );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData->InitialiseDataStructures( );

    Types::UnsignedVector sizeClassPopulationSizes( Parameters::Get( )->GetNumberOfSizeClasses( ), 0 );

    for( unsigned index = 0; index < mLivingVector.size( ); ++index ) {
        Types::IndividualPointer individual = mLivingVector[ index ];
        mHeterotrophData->AddIndividualData( individual );
        sizeClassPopulationSizes[ individual->GetSizeClassIndex( ) ] += 1;
    }

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        mHeterotrophData->AddSizeClassData( sizeClassIndex, sizeClassPopulationSizes[ sizeClassIndex ] );
    }

    mHeterotrophData->NormaliseData( );
    mHeterotrophData->RecordOutputData( );

    return mHeterotrophData->AreHeterotrophsAlive( );
}

unsigned Heterotrophs::GetPopulationSize( ) const {
    return mLivingVector.size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned index ) const {
    return mLivingVector[ index ];
}

void Heterotrophs::CalculateFeedingProbabilities( ) {
    ResetIndividualMatrix( );

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
                if( preyIndex != predatorIndex )
                    effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * mLivingMatrix[ preyIndex ].size( );
                else
                    effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * ( mLivingMatrix[ preyIndex ].size( ) - 1 );

                mHeterotrophData->SetEffectiveSizeClassVolume( predatorIndex, preyIndex, effectiveSizeClassVolume );

                if( effectiveSizeClassVolume > highestEffectiveSizeClassVolume ) {
                    highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
                    coupledSizeClassIndex = preyIndex;
                }
                effectivePreyVolume += effectiveSizeClassVolume;
            }
            mHeterotrophData->SetCoupledSizeClassIndex( predatorIndex, coupledSizeClassIndex );
            mHeterotrophData->SetEffectivePreyVolume( predatorIndex, effectivePreyVolume );
            mHeterotrophData->SetFeedingProbability( predatorIndex, mHeterotrophProcessor->CalculateFeedingProbability( effectivePreyVolume ) );
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
                if( RandomSFMT::Get( )->GetUniform( ) <= mHeterotrophData->GetFeedingProbability( sizeClassIndex ) ) {
                    Types::IndividualPointer predator = GetRandomIndividualFromSizeClass( sizeClassIndex );

                    if( predator != NULL ) {
                        unsigned coupledIndex = mHeterotrophData->GetCoupledSizeClassIndex( sizeClassIndex );

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
    //#pragma omp parallel num_threads(omp_get_num_procs()) default(none) shared(mLivingVector, mHeterotrophProcessor, mNutrient)
    {
        //#pragma omp for schedule(static)
        for( unsigned index = 0; index < mLivingVector.size( ); ++index ) {
            Types::IndividualPointer individual = mLivingVector[ index ];
            double metabolicDeduction = mHeterotrophProcessor->CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                double waste = individual->Metabolise( metabolicDeduction );
                //#pragma omp critical(lock)
                mNutrient->AddToVolume( waste );
                mHeterotrophProcessor->UpdateSizeClassIndex( individual );
            } else StarveToDeath( individual );
        }
    }
    DeleteDead( );
}

void Heterotrophs::Reproduction( ) {
    //#pragma omp parallel num_threads(omp_get_num_procs()) default(none) shared(mLivingVector, mHeterotrophProcessor, mHeterotrophData)
    {
        //#pragma omp for schedule(static)
        for( unsigned index = 0; index < mLivingVector.size( ); ++index ) {
            Types::IndividualPointer potentialParent = mLivingVector[ index ];

            if( potentialParent->GetVolumeActual( ) >= potentialParent->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = potentialParent->Reproduce( mHeterotrophProcessor );

                mHeterotrophProcessor->UpdateSizeClassIndex( potentialParent );
                childIndividual->SetSizeClassIndex( potentialParent->GetSizeClassIndex( ) );
                if( childIndividual->GetHeritableTraits( )->IsValueMutant( Constants::eVolume ) == true ) {
                    mHeterotrophProcessor->UpdateSizeClassIndex( childIndividual );
                    //#pragma omp critical(lock)
                    mHeterotrophData->IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), Constants::eVolume );
                }
                //#pragma omp critical(lock)
                mHeterotrophData->IncrementBirthFrequencies( potentialParent->GetSizeClassIndex( ), childIndividual->GetSizeClassIndex( ) );

                mChildVector.push_back( childIndividual );
            }
        }
    }
    AddChildren( );
}

void Heterotrophs::Starvation( ) {
    ResetIndividualMatrix( );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = mLivingMatrix[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassSubsetSize = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation ) {
                Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                if( individual != NULL )
                    if( RandomSFMT::Get( )->GetUniform( ) <= mHeterotrophProcessor->CalculateStarvationProbability( individual ) )
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

        if( trophicLevel == 0 )
            grazer->SetTrophicLevel( 2 );
        else
            grazer->SetTrophicLevel( ( trophicLevel + 2 ) / 2.0 );

        mNutrient->AddToVolume( waste );
    }
}

void Heterotrophs::FeedFromHeterotrophs( const Types::IndividualPointer predator, unsigned coupledIndex ) {
    Types::IndividualPointer prey = GetRandomIndividualFromSizeClass( coupledIndex, predator );

    if( prey != NULL ) {
        double preyVolume = prey->GetVolumeActual( );
        mHeterotrophData->IncrementCarnivoreFrequencies( predator, prey );

        double waste = predator->ConsumePreyVolume( preyVolume );

        double predatorTrophicLevel = predator->GetTrophicLevel( );
        double preyTrophicLevel = prey->GetTrophicLevel( );

        double trophicLevel = -1;

        if( predatorTrophicLevel == 0 ) {
            if( preyTrophicLevel == 0 )
                trophicLevel = 3;
            else
                trophicLevel = preyTrophicLevel + 1;
        } else {
            if( preyTrophicLevel == 0 )
                trophicLevel = ( predatorTrophicLevel + 3 ) / 2.0;
            else
                trophicLevel = ( predatorTrophicLevel + preyTrophicLevel + 1 ) / 2.0;
        }

        predator->SetTrophicLevel( trophicLevel );
        mNutrient->AddToVolume( waste );
        KillIndividual( prey );
    }
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer individual ) const {
    // TODO - Improve this function. Risk of looping for a long time to randomly select one of few living individuals...
    unsigned sizeClassPopulation = mLivingMatrix[ sizeClassIndex ].size( );
    unsigned sizeClassLivingFrequency = sizeClassPopulation - mSizeClassDeadFrequencies[ sizeClassIndex ];

    if( individual != NULL && individual->GetSizeClassIndex( ) == sizeClassIndex )
        sizeClassLivingFrequency -= 1;

    Types::IndividualPointer randomIndividual = NULL;

    if( sizeClassLivingFrequency > 0 ) {
        while( randomIndividual == NULL || randomIndividual->IsDead( ) == true || randomIndividual == individual ) {
            unsigned randomIndividualIndex = RandomSFMT::Get( )->GetUniformInt( sizeClassPopulation - 1 );
            randomIndividual = mLivingMatrix[ sizeClassIndex ][ randomIndividualIndex ];
        }
    }

    return randomIndividual;
}

void Heterotrophs::DeleteDead( ) {
    for( unsigned individualIndex = 0; individualIndex < mDeadVector.size( ); ++individualIndex )
        DeleteIndividual( mDeadVector[ individualIndex ] );

    mDeadVector.clear( );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex )
        mSizeClassDeadFrequencies[ sizeClassIndex ] = 0;
}

void Heterotrophs::DeleteIndividual( Types::IndividualPointer individual ) {
    // TODO - Speed up by pre-calculating the starting index
    for( unsigned index = 0; index < mLivingVector.size( ); index++ ) {
        if( individual == mLivingVector[ index ] ) {
            mLivingVector.erase( mLivingVector.begin( ) + index );
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
    mSizeClassDeadFrequencies[ individual->GetSizeClassIndex( ) ] += 1;
    mDeadVector.push_back( individual );
}

void Heterotrophs::AddChildren( ) {
    for( unsigned childIndex = 0; childIndex < mChildVector.size( ); ++childIndex ) {
        mLivingVector.push_back( mChildVector[ childIndex ] );
    }
    mChildVector.clear( );
}