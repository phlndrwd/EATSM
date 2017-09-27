#include "Heterotrophs.h"
#include "Individual.h"
#include "HeterotrophProcessor.h"
#include "HeterotrophData.h"
#include "Parameters.h"
#include "Strings.h"
#include "Nutrient.h"
#include "Autotrophs.h"
#include "HeritableTraits.h"
#include "InitialState.h"
#include "RandomSFMT.h"
#include "Timer.h"
#include "DataRecorder.h"

#include <cmath>

Heterotrophs::Heterotrophs( ) {

}

Heterotrophs::Heterotrophs( Types::NutrientPointer nutrient, Types::AutotrophsPointer phytoplankton ) {
    mNutrient = nutrient;
    mPhytoplankton = phytoplankton;
    mHeterotrophProcessor = new HeterotrophProcessor( );
    mHeterotrophData = new HeterotrophData( );

    CreateInitialPopulation( );
}

Heterotrophs::~Heterotrophs( ) {
    delete mHeterotrophProcessor;
    delete mHeterotrophData;

    delete mPhytoplankton;
    delete mNutrient;

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            delete mLivingIndividuals[ sizeClassIndex ][ individualIndex ];
        }
    }
}

void Heterotrophs::Update( ) {
    Feeding( );
    Metabolisation( );
    Reproduction( );
    Starvation( );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData->InitialiseDataStructures( );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassSize = GetSizeClassPopulation( sizeClassIndex );
        for( unsigned individualIndex = 0; individualIndex < sizeClassSize; ++individualIndex ) {
            Types::IndividualPointer individual = mLivingIndividuals[ sizeClassIndex ][ individualIndex ];
            mHeterotrophData->AddIndividualData( individual );
        }
        mHeterotrophData->AddSizeClassData( sizeClassIndex, sizeClassSize );
    }
    mHeterotrophData->NormaliseData( );
    mHeterotrophData->RecordOutputData( );

    return mHeterotrophData->AreHeterotrophsAlive( );
}

void Heterotrophs::CreateInitialPopulation( ) {
    mDeadIndividuals.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );

    if( Parameters::Get( )->GetReadModelState( ) == false ) {
        mLivingIndividuals.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
        unsigned initialPopulationSize = 0;

        double secondaryProducerVolume = Parameters::Get( )->GetSmallestIndividualVolume( ) * Parameters::Get( )->GetPreferredPreyVolumeRatio( );
        unsigned firstPopulatedIndex = mHeterotrophProcessor->FindSizeClassIndexFromVolume( secondaryProducerVolume );
        double individualVolume = Parameters::Get( )->GetSizeClassMidPoint( firstPopulatedIndex );

        double initialHeterotrophVolume = Parameters::Get( )->GetInitialHeterotrophVolume( );

        while( individualVolume <= initialHeterotrophVolume ) {
            initialHeterotrophVolume -= individualVolume;
            Types::IndividualPointer individual = new Individual( mHeterotrophProcessor, individualVolume, firstPopulatedIndex );
            AddToSizeClass( individual );
            ++initialPopulationSize;
        }
        if( initialHeterotrophVolume > 0 ) mNutrient->AddToVolume( initialHeterotrophVolume );
        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize << " individuals." << std::endl;

    } else {
        mLivingIndividuals = InitialState::Get( )->GetHeterotrophs( );
        std::cout << "Heterotrophic size classes initialised with " << InitialState::Get( )->GetInitialPopulationSize( ) << " individuals." << std::endl;
    }
}

void Heterotrophs::Feeding( ) {
    CalculateFeedingProbabilities( );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );

        if( sizeClassPopulation != 0 ) {
            unsigned sizeClassPopulationSubset = mHeterotrophProcessor->RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset; ++potentialEncounterIndex ) {
                if( RandomSFMT::Get( )->GetUniform( ) <= mHeterotrophData->GetFeedingProbability( sizeClassIndex ) ) {
                    Types::IndividualPointer predator = GetRandomIndividualFromSizeClass( sizeClassIndex );

                    if( predator != NULL ) {
                        unsigned coupledIndex = mHeterotrophData->GetCoupledSizeClassIndex( sizeClassIndex );

                        if( coupledIndex == Parameters::Get( )->GetPhytoplanktonSizeClassIndex( ) )
                            FeedFromPhytoplankton( predator );
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
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {

            Types::IndividualPointer individual = mLivingIndividuals[ sizeClassIndex ][ individualIndex ];
            double metabolicDeduction = mHeterotrophProcessor->CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                double waste = individual->Metabolise( metabolicDeduction );
                mNutrient->AddToVolume( waste );
                mHeterotrophProcessor->UpdateSizeClassIndex( individual );
            } else
                StarveToDeath( individual );
        }
    }
    DeleteDead( );
}

void Heterotrophs::Reproduction( ) {
    // Build list of potential parents
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            Types::IndividualPointer potentialParent = mLivingIndividuals[ sizeClassIndex ][ individualIndex ];

            if( potentialParent->GetVolumeActual( ) >= potentialParent->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = potentialParent->Reproduce( mHeterotrophProcessor );

                mHeterotrophProcessor->UpdateSizeClassIndex( potentialParent );
                childIndividual->SetSizeClassIndex( potentialParent->GetSizeClassIndex( ) );
                if( childIndividual->GetHeritableTraits( )->IsValueMutant( Constants::eVolume ) == true ) {
                    mHeterotrophProcessor->UpdateSizeClassIndex( childIndividual );
                    mHeterotrophData->IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), Constants::eVolume );
                }

                mHeterotrophData->IncrementBirthFrequencies( potentialParent->GetSizeClassIndex( ), childIndividual->GetSizeClassIndex( ) );

                AddChild( childIndividual );
            }
        }
    }
    UpdateSizeClasses( );
    AddChildren( );
}

void Heterotrophs::Starvation( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassSize = GetSizeClassPopulation( sizeClassIndex );

        if( sizeClassSize != 0 ) {
            unsigned sizeClassSubsetSize = mHeterotrophProcessor->RoundWithProbability( sizeClassSize * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

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

void Heterotrophs::UpdateSizeClasses( ) {
    Types::IndividualMatrix livingIndividuals;
    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );
    livingIndividuals.resize( numberOfSizeClasses );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses; ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            Types::IndividualPointer individual = mLivingIndividuals[ sizeClassIndex ][ individualIndex ];

            livingIndividuals[ individual->GetSizeClassIndex( ) ].push_back( individual );
        }
    }
    mLivingIndividuals = livingIndividuals;
}

void Heterotrophs::CalculateFeedingProbabilities( ) {
    for( unsigned predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {

        if( GetSizeClassPopulation( predatorIndex ) > 0 ) {
            int coupledSizeClassIndex = Constants::cMissingValue;
            double effectivePreyVolume = 0;
            double highestEffectivePreyVolume = 0;

            for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {
                double effectiveSizeClassVolume = 0;
                // Add the result of the phytoplankton volume - no frequency coefficient.
                if( preyIndex == Parameters::Get( )->GetPhytoplanktonSizeClassIndex( ) ) {
                    effectiveSizeClassVolume = Parameters::Get( )->GetInterSizeClassPreference( predatorIndex, preyIndex ) * mPhytoplankton->GetVolume( );
                }
                // Add the result of the heterotrophs.
                if( preyIndex != predatorIndex ) {
                    effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * GetSizeClassPopulation( preyIndex );
                } else {
                    effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * ( GetSizeClassPopulation( preyIndex ) - 1 );
                }
                mHeterotrophData->SetEffectiveSizeClassVolume( predatorIndex, preyIndex, effectiveSizeClassVolume );

                if( effectiveSizeClassVolume > highestEffectivePreyVolume ) {
                    highestEffectivePreyVolume = effectiveSizeClassVolume;
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

void Heterotrophs::FeedFromPhytoplankton( const Types::IndividualPointer grazer ) {
    bool isPhytoplanktonAvailable = Parameters::Get( )->GetSmallestIndividualVolume( ) < mPhytoplankton->GetVolume( );

    if( isPhytoplanktonAvailable == true ) {
        mPhytoplankton->SubtractFromVolume( Parameters::Get( )->GetSmallestIndividualVolume( ) );
        mHeterotrophData->IncrementVegetarianFrequencies( grazer );

        double waste = grazer->ConsumePreyVolume( Parameters::Get( )->GetSmallestIndividualVolume( ) );
        double trophicLevel = grazer->GetTrophicLevel( );

        if( trophicLevel == 0 ) {
            grazer->SetTrophicLevel( 2 );
        } else {
            grazer->SetTrophicLevel( ( trophicLevel + 2 ) / 2.0 );
        }

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

void Heterotrophs::AddToSizeClass( const Types::IndividualPointer individual ) {
    mLivingIndividuals[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::RemoveFromSizeClass( const Types::IndividualPointer individual ) {
    unsigned sizeClassIndex = individual->GetSizeClassIndex( );

    for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
        if( individual == mLivingIndividuals[ sizeClassIndex ][ individualIndex ] ) {
            mLivingIndividuals[ sizeClassIndex ].erase( mLivingIndividuals[ sizeClassIndex ].begin( ) + individualIndex );
            break;
        }
    }
}

void Heterotrophs::Delete( const Types::IndividualPointer individual ) {
    RemoveFromSizeClass( individual );
    delete individual;
}

void Heterotrophs::KillIndividual( const Types::IndividualPointer individual ) {
    individual->Kill( );
    mDeadIndividuals[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::DeleteDead( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassDeadFrequency( sizeClassIndex ); ++individualIndex ) {
            Delete( mDeadIndividuals[ sizeClassIndex ][ individualIndex ] );
        }
        mDeadIndividuals[ sizeClassIndex ].clear( );
    }
}

void Heterotrophs::StarveToDeath( const Types::IndividualPointer individual ) {
    mNutrient->AddToVolume( individual->GetVolumeActual( ) );
    mHeterotrophData->IncrementStarvedFrequencies( individual->GetSizeClassIndex( ) );
    KillIndividual( individual );
}

unsigned Heterotrophs::GetSizeClassPopulation( const unsigned sizeClassIndex ) const {
    return mLivingIndividuals[ sizeClassIndex ].size( );
}

unsigned Heterotrophs::GetSizeClassDeadFrequency( const unsigned sizeClassIndex ) const {
    return mDeadIndividuals[ sizeClassIndex ].size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned sizeClassIndex, const unsigned individualIndex ) const {
    return mLivingIndividuals[ sizeClassIndex ][ individualIndex ];
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer individual ) const {
    unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );
    unsigned sizeClassLivingFrequency = sizeClassPopulation - GetSizeClassDeadFrequency( sizeClassIndex );

    if( individual != NULL && individual->GetSizeClassIndex( ) == sizeClassIndex )
        sizeClassLivingFrequency -= 1;

    Types::IndividualPointer randomIndividual = NULL;

    if( sizeClassLivingFrequency > 0 ) {
        while( randomIndividual == NULL || randomIndividual->IsDead( ) == true || randomIndividual == individual ) {
            unsigned randomIndividualIndex = RandomSFMT::Get( )->GetUniformInt( sizeClassPopulation - 1 );
            randomIndividual = mLivingIndividuals[ sizeClassIndex ][ randomIndividualIndex ];
        }
    }

    return randomIndividual;
}

void Heterotrophs::AddChild( const Types::IndividualPointer child ) {
    mChildren.push_back( child );
}

void Heterotrophs::AddChildren( ) {
    for( unsigned childIndex = 0; childIndex < mChildren.size( ); ++childIndex ) {
        Types::IndividualPointer childIndividual = mChildren[ childIndex ];
        AddToSizeClass( childIndividual );
    }
    mChildren.clear( );
}
