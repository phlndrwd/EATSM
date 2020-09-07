#include "Heterotrophs.h"

#include "Nutrient.h"
#include "Autotrophs.h"
#include "Parameters.h"
#include "Individual.h"
#include "InitialState.h"
#include "HeritableTraits.h"
#include "DataRecorder.h"
#include "RandomSimple.h"

#include <iostream>
#include <algorithm>

Heterotrophs::Heterotrophs( Nutrient& nutrient, Autotrophs& autotrophs ):
mNutrient( nutrient ), mAutotrophs( autotrophs ), mHeterotrophProcessor( ), mHeterotrophData( ), mTimer( ) {
    CreateInitialPopulation( );
}

Heterotrophs::~Heterotrophs( ) {
    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        for( unsigned int individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex )
            delete mIndividuals[ sizeClassIndex ][ individualIndex ];

        mIndividuals[ sizeClassIndex ].clear( );
    }
    mIndividuals.clear( );
}

void Heterotrophs::CreateInitialPopulation( ) {
    unsigned numberOfSizeClasses = Parameters::Get( )->GetNumberOfSizeClasses( );
    mIndividuals.resize( numberOfSizeClasses );
    mIndividualsDead.resize( numberOfSizeClasses );
    mFedCount.resize( numberOfSizeClasses );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses; ++sizeClassIndex ) {
        unsigned maxSizeClassPopulation = Parameters::Get( )->GetMaximumSizeClassPopulation( sizeClassIndex );
        mIndividuals[ sizeClassIndex ].reserve( maxSizeClassPopulation );
    }

    if( InitialState::Get( )->IsInitialised( ) == false ) {
        unsigned initialPopulationSize = 0;
        double secondaryProducerVolume = Parameters::Get( )->GetSmallestIndividualVolume( ) * Parameters::Get( )->GetPreferredPreyVolumeRatio( );

        unsigned firstPopulatedIndex = mHeterotrophProcessor.FindSizeClassIndexFromVolume( secondaryProducerVolume );
        double individualVolume = Parameters::Get( )->GetSizeClassMidPoint( firstPopulatedIndex );
        double traitValue = mHeterotrophProcessor.VolumeToTraitValue( individualVolume );

        double initialHeterotrophVolume = Parameters::Get( )->GetInitialHeterotrophVolume( );

        while( individualVolume <= initialHeterotrophVolume ) {
            initialHeterotrophVolume -= individualVolume;
            std::vector< double > heritableTraitValues{ traitValue };
            std::vector< bool > areTraitsMutant{ false };
            HeritableTraits heritableTraits( heritableTraitValues, areTraitsMutant );
            //mIndividuals[ firstPopulatedIndex ].emplace_back( heritableTraitValues, individualVolume, firstPopulatedIndex );
            mIndividuals[ firstPopulatedIndex ].push_back( new Individual( heritableTraits, individualVolume, firstPopulatedIndex ) );
            ++initialPopulationSize;
        }

        if( initialHeterotrophVolume > 0 ) mNutrient.AddToVolume( initialHeterotrophVolume );

        std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize + 1 << " individuals." << std::endl;
    } else {
        mIndividuals = InitialState::Get( )->GetHeterotrophs( );
        std::cout << "Heterotrophic size classes initialised with " << InitialState::Get( )->GetInitialPopulationSize( ) << " individuals." << std::endl;
    }
}

void Heterotrophs::Update( ) {
    // Feeding
    mTimer.Start( );
    Feeding( );
    mHeterotrophData.AddToTimeFeeding( mTimer.Stop( ) );
    // Metabolising
    mTimer.Start( );
    Metabolisation( );
    mHeterotrophData.AddToTimeMetabolising( mTimer.Stop( ) );
    // Starving
    mTimer.Start( );
    Starvation( );
    mHeterotrophData.AddToTimeStarving( mTimer.Stop( ) );
    // Reproducing
    mTimer.Start( );
    Reproduction( );
    mHeterotrophData.AddToTimeReproducing( mTimer.Stop( ) );
}

bool Heterotrophs::RecordData( ) {
    mHeterotrophData.InitialiseDataStructures( );
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {

        unsigned sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );
        for( unsigned int individualIndex = 0; individualIndex < sizeClassPopulation; ++individualIndex ) {
            mHeterotrophData.AddIndividualData( mIndividuals[ sizeClassIndex ][ individualIndex ] );
        }
        double sizeClassMultiplier = 1 / ( double ) sizeClassPopulation;
        mHeterotrophData.AddSizeClassData( sizeClassIndex, sizeClassPopulation, sizeClassMultiplier );
    }
    mHeterotrophData.NormaliseData( );
    mHeterotrophData.RecordOutputData( );

    return mHeterotrophData.AreHeterotrophsAlive( );
}

std::size_t Heterotrophs::GetSizeClassPopulation( const unsigned sizeClassIndex ) const {
    return mIndividuals[ sizeClassIndex ].size( );
}

Types::IndividualPointer Heterotrophs::GetIndividual( const unsigned sizeClassIndex, const unsigned individualIndex ) {
    return mIndividuals[ sizeClassIndex ][ individualIndex ];
}

void Heterotrophs::CalculateFeedingProbabilities( ) {
    for( unsigned predatorIndex = 0; predatorIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++predatorIndex ) {
        mFedCount[ predatorIndex ] = 0;
        if( mIndividuals[ predatorIndex ].size( ) > 0 ) {
            unsigned coupledSizeClassIndex = 0;
            double effectivePreyVolume = 0;
            double highestEffectiveSizeClassVolume = 0;

            for( unsigned preyIndex = 0; preyIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++preyIndex ) {
                double effectiveSizeClassVolume = 0;
                // Add the result of the autotroph volume - no frequency coefficient.
                if( preyIndex == Parameters::Get( )->GetAutotrophSizeClassIndex( ) )
                    effectiveSizeClassVolume = Parameters::Get( )->GetInterSizeClassPreference( predatorIndex, preyIndex ) * mAutotrophs.GetVolume( );
                // Add the result of the heterotrophs.
                std::size_t preySizeClassPopulation = GetSizeClassPopulation( preyIndex );
                if( preySizeClassPopulation > 0 ) {
                    if( preyIndex != predatorIndex ) effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * preySizeClassPopulation;
                    else effectiveSizeClassVolume += Parameters::Get( )->GetInterSizeClassVolume( predatorIndex, preyIndex ) * ( preySizeClassPopulation - 1 );
                }
                mHeterotrophData.SetEffectiveSizeClassVolume( predatorIndex, preyIndex, effectiveSizeClassVolume );

                if( effectiveSizeClassVolume > highestEffectiveSizeClassVolume ) {
                    highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
                    coupledSizeClassIndex = preyIndex;
                }
                effectivePreyVolume += effectiveSizeClassVolume;
            }
            mHeterotrophData.SetCoupledSizeClassIndex( predatorIndex, coupledSizeClassIndex );
            mHeterotrophData.SetEffectivePreyVolume( predatorIndex, effectivePreyVolume );
            mHeterotrophData.SetFeedingProbability( predatorIndex, mHeterotrophProcessor.CalculateFeedingProbability( predatorIndex, effectivePreyVolume ) );
        }
    }
}

void Heterotrophs::Feeding( ) {
    CalculateFeedingProbabilities( );

    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        std::size_t sizeClassPopulation = GetSizeClassPopulation( sizeClassIndex );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassPopulationSubset = mHeterotrophProcessor.RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );
            unsigned coupledIndex = mHeterotrophData.GetCoupledSizeClassIndex( sizeClassIndex );

            for( unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset; ++potentialEncounterIndex ) {
                if( RandomSimple::Get( )->GetUniform( ) <= mHeterotrophData.GetFeedingProbability( sizeClassIndex ) ) {

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
        for( std::size_t individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {

            Types::IndividualPointer individual = GetIndividual( sizeClassIndex, individualIndex );

            double metabolicDeduction = mHeterotrophProcessor.CalculateMetabolicDeduction( individual );

            if( ( individual->GetVolumeActual( ) - metabolicDeduction ) > 0 ) {
                individual->SetHasFed( false ); // Reset for the next time step
                double waste = individual->Metabolise( metabolicDeduction );
                mNutrient.AddToVolume( waste );

                // Individuals can move up a size class from having consumed a
                // lot. They need to move after this function has completed to
                // avoid handling them twice.
                if( mHeterotrophProcessor.UpdateSizeClassIndex( individual ) == true )
                    StageForMoving( individual, sizeClassIndex );

            } else Starve( individual );
        }
    }
    MoveIndividuals( );
    DeleteDead( );
}

void Heterotrophs::Starvation( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        unsigned sizeClassPopulation = mIndividuals[ sizeClassIndex ].size( );

        if( sizeClassPopulation > 0 ) {
            unsigned sizeClassSubsetSize = mHeterotrophProcessor.RoundWithProbability( sizeClassPopulation * Parameters::Get( )->GetSizeClassSubsetFraction( ) );

            for( unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation ) {
                Types::IndividualPointer individual = GetRandomIndividualFromSizeClass( sizeClassIndex );

                if( individual != NULL )
                    if( RandomSimple::Get( )->GetUniform( ) <= mHeterotrophProcessor.CalculateStarvationProbability( individual ) )
                        Starve( individual );
            }
        }
    }
    DeleteDead( );
}

void Heterotrophs::Reproduction( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( std::size_t individualIndex = 0; individualIndex < GetSizeClassPopulation( sizeClassIndex ); ++individualIndex ) {

            Types::IndividualPointer individual = GetIndividual( sizeClassIndex, individualIndex );
            if( individual->GetVolumeActual( ) >= individual->GetVolumeReproduction( ) ) {
                Types::IndividualPointer childIndividual = individual->Reproduce( );

                // Parent data needs to be collected before size class is updated.
                mHeterotrophData.IncrementParentFrequencies( individual->GetSizeClassIndex( ) );
                if( mHeterotrophProcessor.UpdateSizeClassIndex( individual ) == true )
                    StageForMoving( individual, sizeClassIndex );

                if( childIndividual->GetHeritableTraits( ).IsTraitMutant( Constants::eVolume ) == true ) {
                    mHeterotrophProcessor.UpdateSizeClassIndex( childIndividual ); // Mutants may inherit more than a fixed fraction of the parent's volume
                    mHeterotrophData.IncrementMutantFrequency( childIndividual->GetSizeClassIndex( ), Constants::eVolume );
                }

                mHeterotrophData.IncrementChildFrequencies( childIndividual->GetSizeClassIndex( ) );
                mChildren.push_back( childIndividual );
            }
        }
    }
    MoveIndividuals( );
    AddChildren( );
}

void Heterotrophs::FeedFromAutotrophs( Types::IndividualPointer grazer ) {
    double smallestIndividualVolume = Parameters::Get( )->GetSmallestIndividualVolume( );

    if( mAutotrophs.GetVolume( ) > smallestIndividualVolume ) {
        mAutotrophs.SubtractFromVolume( smallestIndividualVolume );
        mHeterotrophData.IncrementVegetarianFrequencies( grazer );

        double waste = grazer->ConsumePreyVolume( smallestIndividualVolume );
        ++mFedCount[ grazer->GetSizeClassIndex( )];

        double trophicLevel = grazer->GetTrophicLevel( );
        if( trophicLevel != 0 ) grazer->SetTrophicLevel( ( trophicLevel + 2 ) * 0.5 );
        else grazer->SetTrophicLevel( 2 );
        mNutrient.AddToVolume( waste );
    }
}

void Heterotrophs::FeedFromHeterotrophs( Types::IndividualPointer predator, unsigned coupledIndex ) {
    Types::IndividualPointer prey = GetRandomPreyFromSizeClass( coupledIndex, predator );
    if( prey != NULL ) {
        double preyVolume = prey->GetVolumeActual( );
        mHeterotrophData.IncrementCarnivoreFrequencies( predator, prey );

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
        mNutrient.AddToVolume( waste );
        Kill( prey );
    }
}

Types::IndividualPointer Heterotrophs::GetRandomIndividualFromSizeClass( const unsigned sizeClassIndex ) {
    std::size_t numberLiving = GetSizeClassPopulation( sizeClassIndex );
    unsigned numberActive = numberLiving - mIndividualsDead[ sizeClassIndex ].size( );

    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        unsigned count = 0;
        do {
            ++count;
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberLiving - 1 );
            randomIndividual = GetIndividual( sizeClassIndex, randomIndividualIndex );
            if( count > numberActive ) break;
        } while( randomIndividual->IsDead( ) == true );
    }
    return randomIndividual;
}

Types::IndividualPointer Heterotrophs::GetRandomPredatorFromSizeClass( const unsigned sizeClassIndex ) {
    std::size_t numberLiving = GetSizeClassPopulation( sizeClassIndex );
    unsigned numberActive = numberLiving - mIndividualsDead[ sizeClassIndex ].size( ) - mFedCount[ sizeClassIndex ];

    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        unsigned count = 0;
        do {
            ++count;
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberLiving - 1 );
            randomIndividual = GetIndividual( sizeClassIndex, randomIndividualIndex );
            if( count > numberActive ) break;
        } while( randomIndividual->IsDead( ) == true || randomIndividual->HasFed( ) );
    }
    return randomIndividual;
}

Types::IndividualPointer Heterotrophs::GetRandomPreyFromSizeClass( const unsigned sizeClassIndex, const Types::IndividualPointer predator ) {
    std::size_t numberLiving = GetSizeClassPopulation( sizeClassIndex );
    unsigned numberActive = numberLiving - mIndividualsDead[ sizeClassIndex ].size( );

    // Only applicable when predator feeds from its own size class. Check 
    // ensures numberActive does not go out of bounds
    if( predator->GetSizeClassIndex( ) == sizeClassIndex && numberActive > 0 )
        --numberActive;

    Types::IndividualPointer randomIndividual = NULL;
    if( numberActive > 0 ) {
        unsigned count = 0;
        do {
            ++count;
            unsigned randomIndividualIndex = RandomSimple::Get( )->GetUniformInt( numberLiving - 1 );
            randomIndividual = GetIndividual( sizeClassIndex, randomIndividualIndex );
            if( count > numberActive ) break;
        } while( randomIndividual->IsDead( ) == true || randomIndividual == predator );
    }
    return randomIndividual;
}

void Heterotrophs::StageForMoving( Types::IndividualPointer individual, const unsigned currentSizeClassIndex ) {
    mIndividualsToMove.push_back( std::make_pair( individual, currentSizeClassIndex ) );
}

void Heterotrophs::MoveSizeClass( Types::IndividualPointer individual, const unsigned currentSizeClassIndex ) {
    if( RemoveFromSizeClass( individual, currentSizeClassIndex ) == true )
        mIndividuals[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

bool Heterotrophs::RemoveFromSizeClass( Types::IndividualPointer individual, const unsigned sizeClassIndex ) {
    std::vector< Types::IndividualPointer >::iterator toErase;
    toErase = std::find( mIndividuals[ sizeClassIndex ].begin( ), mIndividuals[ sizeClassIndex ].end( ), individual );

    // And then erase if found
    if( toErase != mIndividuals[ sizeClassIndex ].end( ) ) {
        // Delete when necessary in calling function
        mIndividuals[ sizeClassIndex ].erase( toErase );
        return true;
    }
    return false;
}

void Heterotrophs::DeleteIndividual( Types::IndividualPointer individual ) {
    RemoveFromSizeClass( individual, individual->GetSizeClassIndex( ) );
    delete individual;
}

void Heterotrophs::Starve( Types::IndividualPointer individual ) {
    mNutrient.AddToVolume( individual->GetVolumeActual( ) );
    mHeterotrophData.IncrementStarvedFrequencies( individual->GetSizeClassIndex( ) );
    Kill( individual );
}

void Heterotrophs::Kill( Types::IndividualPointer individual ) {
    individual->Kill( );
    mIndividualsDead[ individual->GetSizeClassIndex( ) ].push_back( individual );
}

void Heterotrophs::AddChildren( ) {
    for( std::size_t childIndex = 0; childIndex < mChildren.size( ); ++childIndex ) {
        Types::IndividualPointer child = mChildren[ childIndex ];
        mIndividuals[ child->GetSizeClassIndex( ) ].push_back( child );
    }
    mChildren.clear( );
}

void Heterotrophs::MoveIndividuals( ) {
    for( unsigned index = 0; index < mIndividualsToMove.size( ); ++index ) {
        MoveSizeClass( mIndividualsToMove[ index ].first, mIndividualsToMove[ index ].second );
    }
    mIndividualsToMove.clear( );
}

void Heterotrophs::DeleteDead( ) {
    for( unsigned sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned individualIndex = 0; individualIndex < mIndividualsDead[ sizeClassIndex ].size( ); ++individualIndex )
            DeleteIndividual( mIndividualsDead[ sizeClassIndex ][ individualIndex ] );

        mIndividualsDead[ sizeClassIndex ].clear( );
    }
}