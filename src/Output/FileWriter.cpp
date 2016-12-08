#include <ios>
#include <limits>

#include "FileWriter.h"
#include "DataRecorder.h"
#include "Convertor.h"
#include "Parameters.h"
#include "Logger.h"
#include "Date.h"
#include "VectorDatum.h"
#include "MatrixDatum.h"
#include "Environment.h"
#include "Nutrient.h"
#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "Individual.h"
#include "Genome.h"
#include "Tagger.h"
#include "DataTag.h"
#include "ConsumptionEvent.h"
#include "RandomSFMT.h"

FileWriter::FileWriter( ) {
    InitialiseOutputDirectory( );
    WriteInputFiles( );
}

FileWriter::~FileWriter( ) {

}

void FileWriter::InitialiseOutputDirectory( ) {
    // output/
    mOutputPath = Constants::cOutputDirectoryName;
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

    // output/[version]/
    mOutputPath.append( Constants::cSystemVersion );
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

    // output/[version]/[experiment]/[date and time]
    Date date;
    mDataSetDirectoryName = date.GetDateAndTimeString( Constants::cDataSetNameFormat );

    mOutputPath.append( mDataSetDirectoryName );
    int returnValue = mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );

    // The following code ensures the data are written into a unique subdirectory.
    if( returnValue == -1 ) {
        mOutputPath.append( "_" );
        int stringLength = mOutputPath.length( );

        int count = 1;
        while( returnValue == -1 ) {
            mOutputPath.replace( stringLength, 1, Convertor::Get( )->ToString( count ) );
            returnValue = mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
            ++count;
        }
    }

    mOutputPath.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );
}

void FileWriter::WriteInputFiles( ) {
    Types::StringVector inputFilePaths = DataRecorder::Get( )->GetInputFilePaths( );

    for( unsigned stringIndex = 0; stringIndex < inputFilePaths.size( ); ++stringIndex ) {
        std::ifstream sourceFileStream( inputFilePaths[ stringIndex ].c_str( ), std::ios::in );

        std::string outputFilePath = mOutputPath;
        Types::StringVector inputFilePathComponents = Convertor::Get( )->StringToWords( inputFilePaths[ stringIndex ], Constants::cFolderDelimiter );

        std::string fileName = inputFilePathComponents[ inputFilePathComponents.size( ) - 1 ];
        outputFilePath.append( fileName );

        std::ofstream destinationFileStream( outputFilePath.c_str( ), std::ios::out );

        if( fileName != Constants::cInputParametersFileName )
            destinationFileStream << sourceFileStream.rdbuf( );
        else {
            // This routine is specific to the input parameters file and is added to record a random seed value with the output.
            if( sourceFileStream.is_open( ) ) {
                std::string readLine;
                unsigned lineCount = 0;

                while( std::getline( sourceFileStream, readLine ) ) {
                    if( lineCount > 0 ) {
                        Types::StringVector readWords = Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue );
                        std::string parameterName = Convertor::Get( )->RemoveWhiteSpace( readWords[ Constants::eParameterName ] );

                        if( Convertor::Get( )->ToLowercase( parameterName ) != "randomseed" )
                            destinationFileStream << readLine << std::endl;
                        else
                            destinationFileStream << parameterName << Constants::cDataDelimiterValue << Parameters::Get( )->GetRandomSeed( ) << std::endl;

                    } else if( lineCount == 0 ) {
                        destinationFileStream << readLine << std::endl;
                    }
                    ++lineCount;
                }
            }
        }
        sourceFileStream.close( );
        destinationFileStream.close( );
    }
}

void FileWriter::WriteOutputData( Types::EnvironmentPointer environment ) {
    // Write vector datums
    bool success = true;
    Types::VectorDatumMap vectorDatumMap = DataRecorder::Get( )->GetVectorDatumMap( );

    for( Types::VectorDatumMap::iterator iter = vectorDatumMap.begin( ); iter != vectorDatumMap.end( ); ++iter ) {
        std::string fileName = iter->first;
        Types::VectorDatumPointer vectorDatum = iter->second;
        fileName.insert( 0, mOutputPath ).append( Constants::cFileNameExtension );
        std::ofstream outputFileStream;
        outputFileStream.open( fileName.c_str( ), std::ios::out );
        if( outputFileStream.is_open( ) == true ) {
            for( unsigned dataIndex = 0; dataIndex < vectorDatum->GetSize( ) - 1; ++dataIndex ) {
                outputFileStream << vectorDatum->GetDataAtIndex( dataIndex ) << Constants::cDataDelimiterValue;
            }
            outputFileStream << vectorDatum->GetDataAtIndex( vectorDatum->GetSize( ) - 1 );
            outputFileStream.close( );
        } else {
            success = false;
        }
    }
    // Write grid datums
    if( success == true ) {
        success = true;
        Types::MatrixDatumMap matrixDatumMap = DataRecorder::Get( )->GetMatrixDatumMap( );
        for( Types::MatrixDatumMap::iterator iter = matrixDatumMap.begin( ); iter != matrixDatumMap.end( ); ++iter ) {

            std::string fileName = iter->first;
            Types::MatrixDatumPointer matrixDatum = iter->second;
            fileName.insert( 0, mOutputPath ).append( Constants::cFileNameExtension );
            std::ofstream outputFileStream;
            outputFileStream.open( fileName.c_str( ), std::ios::out );
            if( outputFileStream.is_open( ) == true ) {
                for( unsigned rowIndex = 0; rowIndex < matrixDatum->GetRows( ); ++rowIndex ) {
                    for( unsigned columnIndex = 0; columnIndex < matrixDatum->GetColumns( ) - 1; ++columnIndex ) {
                        outputFileStream << matrixDatum->GetDataAtIndices( rowIndex, columnIndex ) << Constants::cDataDelimiterValue;
                    }
                    outputFileStream << matrixDatum->GetDataAtIndices( rowIndex, matrixDatum->GetColumns( ) - 1 ) << std::endl;
                }
                outputFileStream.close( );
            } else
                success = false;
        }
    }
    // Write tag data
    if( success == true ) {
        success = true;
        if( Parameters::Get( )->GetPopulationTagPercentage( ) > 0 ) {
            Types::TaggerPointer tagger = environment->GetHeterotrophs( )->GetTagger( );

            for( unsigned int tagIndex = 0; tagIndex < tagger->GetNumberOfTags( ); ++tagIndex ) {
                Types::DataTagPointer tag = tagger->GetTag( tagIndex );

                std::string outputSubdirectory = mOutputPath;
                outputSubdirectory.append( Constants::cTagFileName + Convertor::Get( )->ToString( tag->GetID( ) ) );
                mkdir( outputSubdirectory.c_str( ), Constants::cOutputFolderPermissions );
                outputSubdirectory.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

                // Write tag attributes
                std::string fileName = outputSubdirectory;
                fileName.append( Constants::cAttributesFileName );
                std::ofstream outputFileStream;
                outputFileStream.open( fileName.c_str( ), std::ios::out );

                if( outputFileStream.is_open( ) == true ) {
                    Types::FloatMap& attributeMap = tag->GetAttributes( );

                    for( Types::FloatMap::iterator iter = attributeMap.begin( ); iter != attributeMap.end( ); ++iter ) {
                        outputFileStream << iter->first << Constants::cDataDelimiterValue << iter->second << std::endl;
                    }
                    outputFileStream.close( );
                } else
                    success = false;

                // Write tag time series
                Types::FloatVectorMap timeSeriesDataMap = tag->GetTimeSeriesData( );

                for( Types::FloatVectorMap::iterator iter = timeSeriesDataMap.begin( ); iter != timeSeriesDataMap.end( ); ++iter ) {
                    std::string fileName = outputSubdirectory;

                    fileName.append( iter->first );
                    fileName.append( Constants::cFileNameExtension );
                    std::ofstream outputFileStream;
                    outputFileStream.open( fileName.c_str( ), std::ios::out );

                    if( outputFileStream.is_open( ) == true ) {
                        Types::FloatVector& data = iter->second;

                        for( unsigned index = 0; index < data.size( ) - 1; ++index ) {
                            outputFileStream << data[ index ] << Constants::cDataDelimiterValue;
                        }
                        outputFileStream << data[ data.size( ) - 1 ] << std::endl;
                        outputFileStream.close( );
                    } else
                        success = false;
                }
                // Write tag herbivory consumption events
                Types::ConsumptionEventVector& herbivoryEvents = tag->GetHerbivoryEvents( );
                if( herbivoryEvents.size( ) > 0 ) {
                    std::string fileName = outputSubdirectory; // Reused variable
                    fileName.append( Constants::cHerbivoryEventsFileName );
                    std::ofstream outputFileStream;
                    outputFileStream.open( fileName.c_str( ), std::ios::out ); // Reused variable

                    if( outputFileStream.is_open( ) == true ) {
                        outputFileStream << "TimeStep,PreyVolume,VolumeAssimilated,WasteVolume" << std::endl;
                        for( unsigned index = 0; index < herbivoryEvents.size( ); ++index ) {
                            outputFileStream << herbivoryEvents[ index ]->GetTimeStep( ) << Constants::cDataDelimiterValue << herbivoryEvents[ index ]->GetPreyVolume( ) << Constants::cDataDelimiterValue << herbivoryEvents[ index ]->GetVolumeAssimilated( ) << Constants::cDataDelimiterValue << herbivoryEvents[ index ]->GetWasteVolume( ) << std::endl;
                        }
                        outputFileStream.close( );
                    } else
                        success = false;
                }
                // Write tag carnivory consumption events
                Types::ConsumptionEventVector& carnivoryEvents = tag->GetCarnivoryEvents( );
                if( carnivoryEvents.size( ) > 0 ) {
                    std::string fileName = outputSubdirectory; // Reused variable
                    fileName.append( Constants::cCarnivoryEventsFileName );
                    std::ofstream outputFileStream;
                    outputFileStream.open( fileName.c_str( ), std::ios::out ); // Reused variable

                    if( outputFileStream.is_open( ) == true ) {
                        outputFileStream << "TimeStep,PreyVolume,VolumeAssimilated,WasteVolume" << std::endl;
                        for( unsigned index = 0; index < carnivoryEvents.size( ); ++index ) {
                            outputFileStream << carnivoryEvents[ index ]->GetTimeStep( ) << Constants::cDataDelimiterValue << carnivoryEvents[ index ]->GetPreyVolume( ) << Constants::cDataDelimiterValue << carnivoryEvents[ index ]->GetVolumeAssimilated( ) << Constants::cDataDelimiterValue << carnivoryEvents[ index ]->GetWasteVolume( ) << std::endl;
                        }
                        outputFileStream.close( );
                    } else
                        success = false;
                }
            }
        }
    }
    // Write state file
    if( success == true ) {
        success = true;
        if( Parameters::Get( )->GetWriteModelState( ) == true ) {

            std::string fileName = mOutputPath;
            fileName.append( Constants::cModelStateFileName );

            std::ofstream modelStateFileStream;
            modelStateFileStream.open( fileName.c_str( ), std::ios::out );

            //modelStateFileStream.flags( std::ios::scientific );
            modelStateFileStream.precision( std::numeric_limits< double >::digits );

            if( modelStateFileStream.is_open( ) == true ) {
                // Header (for consistency with general file reading function)
                modelStateFileStream << Constants::cModelStateFileName << std::endl;
                // Use mother-of-all flag
                modelStateFileStream << RandomSFMT::Get( )->GetUseMother( ) << std::endl;
                // Random state index
                modelStateFileStream << RandomSFMT::Get( )->GetStateIndex( ) << std::endl;
                // Random (mother-of-all) state
                for( unsigned index = 0; index < MOA_N - 1; ++index ) {
                    modelStateFileStream << RandomSFMT::Get( )->GetMotherState( index ) << Constants::cDataDelimiterValue;
                }
                modelStateFileStream << RandomSFMT::Get( )->GetMotherState( MOA_N - 1 ) << std::endl;
                // Random (SFMT) state
                for( unsigned index = 0; index < SFMT_N; ++index ) {
                    modelStateFileStream << Convertor::Get( )->M128iToString< unsigned >( RandomSFMT::Get( )->GetState( index ) ) << std::endl;
                }
                // Model variables
                modelStateFileStream << environment->GetNutrient( )->GetVolume( ) << std::endl;
                modelStateFileStream << environment->GetAutotrophs( )->GetVolume( ) << std::endl;

                Types::HeterotrophsPointer heterotrophs = environment->GetHeterotrophs( );

                for( unsigned int populationIndex = 0; populationIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++populationIndex ) {
                    for( unsigned int individualIndex = 0; individualIndex < heterotrophs->GetSizeClassPopulation( populationIndex ); ++individualIndex ) {
                        Types::IndividualPointer individual = heterotrophs->GetIndividual( populationIndex, individualIndex );
                        modelStateFileStream << individual->GetSizeClassIndex( ) << Constants::cDataDelimiterValue << individual->GetGenome( )->GetGeneValue( Constants::eVolumeGene ) << Constants::cDataDelimiterValue << individual->GetVolumeActual( ) << std::endl;
                    }
                }
                modelStateFileStream.close( );
            } else
                success = false;
        }
    }
    if( success == true ) Logger::Get( )->LogMessage( "Output data written to \"" + mOutputPath + "\"." );
    else Logger::Get( )->LogMessage( "ERROR> File writing failed. Could not access \"" + mOutputPath + "\"." );
}
