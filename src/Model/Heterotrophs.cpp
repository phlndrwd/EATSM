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

    DataRecorder::Get( )->InitialiseMatrix( "Timing", 4 );
}

Heterotrophs::~Heterotrophs( ) {
    delete mHeterotrophProcessor;
    delete mHeterotrophData;

    delete mPhytoplankton;
    delete mNutrient;

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            delete mSizeClasses[ sizeClassIndex ][ individualIndex ];
        }
    }
}

void Heterotrophs::Update( ) {
    Timer timer( false );

    double feedingTime = 0;
    double metabolismTime = 0;
    double starvationTime = 0;
    double reproductionTime = 0;

    timer.Start( );
    Feeding( );
    feedingTime = timer.Stop( );

    timer.Start( );
    Metabolisation( );
    metabolismTime = timer.Stop( );

    timer.Start( );
    Starvation( );
    starvationTime = timer.Stop( );

    timer.Start( );
    Reproduction( );
    reproductionTime = timer.Stop( );

    double totalTime = feedingTime + metabolismTime + starvationTime + reproductionTime;

    feedingTime = feedingTime / totalTime;
    metabolismTime = metabolismTime / totalTime;
    starvationTime = starvationTime / totalTime;
    reproductionTime = reproductionTime / totalTime;

    DataRecorder::Get( )->AddDataTo( "Timing", 0, feedingTime );
    DataRecorder::Get( )->AddDataTo( "Timing", 1, metabolismTime );
    DataRecorder::Get( )->AddDataTo( "Timing", 2, starvationTime );
    DataRecorder::Get( )->AddDataTo( "Timing", 3, reproductionTime );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData->InitialiseDataStructures( );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassSize = GetSizeClassPopulation( sizeClassIndex );
        for( unsigned individualIndex = 0; individualIndex < sizeClassSize; ++individualIndex ) {
            Types::IndividualPointer individual = mSizeClasses[ sizeClassIndex ][ individualIndex ];
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
        mSizeClasses.resize( Parameters::Get( )->GetNumberOfSizeClasses( ) );
        unsigned initialPopulationSize = 0;

        double secondaryProducerVolume = Parameters::Get( )->GetSmallestIndividualVolume( ) * Parameters::Get( )->GetPreferredPreyVolumeRatio( );
        unsigned firstPopulatedIndex = mHeterotrophProcessor->FindSizeClassIndexFromVolume( secondaryProducerVolume ) - 1;

        double initialHeterotrophVolume = Parameters::Get( )->GetInitialHeterotrophVolume( );

        double individualVolume = Parameters::Get( )->GetSizeClassMidPoint( firstPopulatedIndex );

        while( individualVolume <= initialHeterotrophVolume ) {
            initialHeterotrophVolume -= individualVolume;
            Types::IndividualPointer individual = new Individual( mHeterotrophProcessor, individualVolume, firstPopulatedIndex );
            AddToSizeClass( individual );
            ++initialPopulationSize;
        }
        if( initialHeterotrophVolume > 0 ) mNutrient->AddToVolume( initialHeterotrophVolume );
        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize << " individuals." << std::endl;

    } else {
        mSizeClasses = InitialState::Get( )->GetHeterotrophs( );
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
                    Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                    if( individual != 0 ) {
                        unsigned coupledIndex = mHeterotrophData->GetCoupledSizeClassIndex( sizeClassIndex );

                        if( coupledIndex == Parameters::Get( )->GetPhytoplanktonSizeClassIndex( ) ) {
                            FeedFromPhytoplankton( individual );
                        } else {
                            FeedFromHeterotrophs( individual, coupledIndex );
                        }
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

            Types::IndividualPointer individual = mSizeClasses[ sizeClassIndex ][ individualIndex ];
            double metabolicDeduction = mHeterotrophProcessor->CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                double waste = individual->Metabolise( metabolicDeduction );
                mNutrient->AddToVolume( waste );

                unsigned directionToMove = mHeterotrophProcessor->DirectionIndividualShouldMoveSizeClasses( individual );
                if( directionToMove != Constants::eNoMovement )
                    MoveSizeClass( individual, directionToMove );
            } else
                StarveToDeath( individual );
        }
    }
    DeleteDead( );
}

void Heterotrophs::Starvation( ) {
    if( Parameters::Get( )->GetApplyStarvationFunction( ) == true ) {
        for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
            unsigned sizeClassSize = GetSizeClassPopulation( sizeClassIndex );

            if( sizeClassSize != 0 ) {
                unsigned sizeClassSubsetSize = mHeterotrophProcessor->RoundWithProbability( sizeClassSize * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

                for( unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation ) {
                    Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                    if( individual != 0 )
                        if( RandomSFMT::Get( )->GetUniform( ) <= mHeterotrophProcessor->CalculateStarvationProbability( individual ) )
                            StarveToDeath( individual );
                }
            }
        }
        DeleteDead( );
    }
}

void Heterotrophs::Reproduction( ) {
    // Build list of potential parents
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
            Types::IndividualPointer potentialParent = mSizeClasses[ sizeClassIndex ][ individualIndex ];

            if( potentialParent->GetVolumeActual( ) >= potentialParent->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = potentialParent->Reproduce( mHeterotrophProcessor );

                unsigned directionToMove = mHeterotrophProcessor->DirectionIndividualShouldMoveSizeClasses( potentialParent );
                if( directionToMove != Constants::eNoMovement )
                    MoveSizeClass( potentialParent, directionToMove );

                childIndividual->SetSizeClassIndex( potentialParent->GetSizeClassIndex( ) );
                Types::BoolVector isMutant = childIndividual->GetHeritableTraits( )->IsMutant( );
                if( isMutant[ Constants::eVolume ] == true ) {
                    unsigned directionToMove = mHeterotrophProcessor->DirectionIndividualShouldMoveSizeClasses( childIndividual );
                    if( directionToMove != Constants::eNoMovement )
                        mHeterotrophProcessor->FindAndSetSizeClassIndex( childIndividual, directionToMove );
                }

                mHeterotrophData->IncrementBirthFrequencies( potentialParent->GetSizeClassIndex( ), childIndividual->GetSizeClassIndex( ) );
                for( unsigned geneIndex = 0; geneIndex < isMutant.size( ); ++geneIndex )
                    if( isMutant[ geneIndex ] == true )
                        mHeterotrophData->IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), geneIndex );

                AddChild( childIndividual );
            }
        }
    }
    AddChildren( );
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

        double waste = predator->ConsumePreyVolume( preyVolume, false );

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
    mSizeClasses[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::RemoveFromSizeClass( const Types::IndividualPointer individual ) {
    unsigned sizeClassIndex = individual->GetSizeClassIndex( );

    for( unsigned individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {
        if( individual == mSizeClasses[ sizeClassIndex ][ individualIndex ] ) {
            mSizeClasses[ sizeClassIndex ].erase( mSizeClasses[ sizeClassIndex ].begin( ) + individualIndex );
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

void Heterotrophs::MoveSizeClass( const Types::IndividualPointer individual, unsigned direction ) {
    RemoveFromSizeClass( individual );
    mHeterotrophProcessor->FindAndSetSizeClassIndex( individual, direction );
    AddToSizeClass( individual );
}

unsigned Heterotrophs::GetSizeClassPopulation( const unsigned sizeClassIndex ) const {
    return mSizeClasses[ sizeClassIndex ].size( );
}

unsigned Heterotrophs::GetSizeClassDeadFrequency( const unsigned sizeClassIndex ) const {
    return mDeadIndividuals[ sizeClassIndex ].size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned sizeClassIndex, const unsigned individualIndex ) const {
    return mSizeClasses[ sizeClassIndex ][ individualIndex ];
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer individual ) const {
    unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );
    unsigned sizeClassLivingFrequency = sizeClassPopulation - GetSizeClassDeadFrequency( sizeClassIndex );

    if( individual != NULL ) {
        if( individual->GetSizeClassIndex( ) == sizeClassIndex ) {
            sizeClassLivingFrequency = sizeClassLivingFrequency - 1;
        }
    }

    Types::IndividualPointer randomIndividual = NULL;

    if( sizeClassLivingFrequency > 0 ) {
        unsigned randomIndividualIndex = RandomSFMT::Get( )->GetUniformInt( sizeClassPopulation - 1 );
        randomIndividual = mSizeClasses[ sizeClassIndex ][ randomIndividualIndex ];

        while( randomIndividual->IsDead( ) == true || randomIndividual == individual ) {

            randomIndividualIndex = RandomSFMT::Get( )->GetUniformInt( sizeClassPopulation - 1 );
            randomIndividual = mSizeClasses[ sizeClassIndex ][ randomIndividualIndex ];
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
