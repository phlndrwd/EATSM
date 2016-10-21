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
    bool success = false;
    Types::VectorDatumMap vectorDatumMap = DataRecorder::Get( )->GetVectorDatumMap( );

    for( Types::VectorDatumMap::iterator iter = vectorDatumMap.begin( ); iter != vectorDatumMap.end( ); ++iter ) {
        std::string fileName = iter->first;
        Types::VectorDatumPointer vectorDatum = iter->second;
        fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
        std::ofstream outputFileStream;
        outputFileStream.open( fileName.c_str( ), std::ios::out );
        if( outputFileStream.is_open( ) == true ) {
            for( unsigned dataIndex = 0; dataIndex < vectorDatum->GetSize( ) - 1; ++dataIndex ) {
                outputFileStream << vectorDatum->GetDataAtIndex( dataIndex ) << Constants::cDataDelimiterValue;
            }
            outputFileStream << vectorDatum->GetDataAtIndex( vectorDatum->GetSize( ) - 1 );
            outputFileStream.close( );
            success = true;
        }
    }
    // Write grid datums
    if( success == true ) {
        Types::MatrixDatumMap matrixDatumMap = DataRecorder::Get( )->GetMatrixDatumMap( );
        for( Types::MatrixDatumMap::iterator iter = matrixDatumMap.begin( ); iter != matrixDatumMap.end( ); ++iter ) {

            std::string fileName = iter->first;
            Types::MatrixDatumPointer matrixDatum = iter->second;
            fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
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
                success = true;
            } else
                success = false;
        }
    }

    if( success == true ) {
        success = true;

        if( Parameters::Get( )->GetPopulationTagPercentage( ) ) {
            Types::TaggerPointer tagger = environment->GetHeterotrophs( )->GetTagger( );

            for( unsigned int tagIndex = 0; tagIndex < tagger->GetNumberOfTags( ); ++tagIndex ) {

                Types::DataTagPointer tag = tagger->GetTag( tagIndex );

                std::string outputSubdirectory = mOutputPath;
                outputSubdirectory.append( "Tag_" + Convertor::Get( )->ToString( tag->GetID( ) ) );
                mkdir( outputSubdirectory.c_str( ), Constants::cOutputFolderPermissions );
                outputSubdirectory.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

                std::string fileName = outputSubdirectory;
                fileName.append( "Volume" );
                fileName.append( Constants::cOutputFileExtension );

                std::ofstream volumeFileStream;
                volumeFileStream.open( fileName.c_str( ), std::ios::out );

                if( volumeFileStream.is_open( ) == true ) {

                    for( unsigned dataIndex = 0; dataIndex < tag->GetSize( ) - 1; ++dataIndex ) {
                        volumeFileStream << tag->GetVolume( dataIndex ) << Constants::cDataDelimiterValue;
                    }
                    volumeFileStream << tag->GetVolume( tag->GetSize( ) - 1 ) << std::endl;
                    volumeFileStream.close( );

                } else
                    success = false;
            }
        }
    }

    // Write state file
    if( success == true ) {
        if( Parameters::Get( )->GetWriteModelState( ) == true ) {
            std::string fileName = Constants::cModelStateFileName;
            fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
            std::ofstream modelStateFileStream;
            modelStateFileStream.open( fileName.c_str( ), std::ios::out );

            modelStateFileStream.flags( std::ios::scientific );
            modelStateFileStream.precision( std::numeric_limits< double >::digits10 + 1 );

            if( modelStateFileStream.is_open( ) == true ) {
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
                success = true;
            } else
                success = false;
        }
    }
    if( success == true ) Logger::Get( )->LogMessage( "Output data written to \"" + mOutputPath + "\"." );
    else Logger::Get( )->LogMessage( "ERROR> File writing failed. Could not access \"" + mOutputPath + "\"." );
}
