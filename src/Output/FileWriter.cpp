#include <ios>
#include <limits>

#include "FileWriter.h"
#include "DataRecorder.h"
#include "Strings.h"
#include "Parameters.h"
#include "Date.h"
#include "VectorDatum.h"
#include "MatrixDatum.h"
#include "Environment.h"
#include "Nutrient.h"
#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "Individual.h"
#include "HeritableTraits.h"
#include "Tagger.h"
#include "DataTag.h"
#include "ConsumptionEvent.h"
#include "RandomSFMT.h"

FileWriter::FileWriter( ) {
    InitialiseOutputDirectory( );
    if( !WriteInputFiles( ) ) {
        std::cout << "ERROR> Could not write input files to \"" << mOutputPath << "\". System exiting..." << std::endl;
        exit( 1 );
    }
}

FileWriter::~FileWriter( ) {

}

void FileWriter::InitialiseOutputDirectory( ) {
    // output/
    mOutputPath = Constants::cOutputDirectoryName;
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Strings::Get( )->ToString( Constants::cFolderDelimiter ) );

    // output/[version]/
    mOutputPath.append( Constants::cSystemVersion );
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Strings::Get( )->ToString( Constants::cFolderDelimiter ) );

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
            mOutputPath.replace( stringLength, 1, Strings::Get( )->ToString( count ) );
            returnValue = mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
            ++count;
        }
    }

    mOutputPath.append( Strings::Get( )->ToString( Constants::cFolderDelimiter ) );
}

bool FileWriter::WriteInputFiles( ) {
    Types::StringVector inputFilePaths = DataRecorder::Get( )->GetInputFilePaths( );

    for( unsigned stringIndex = 0; stringIndex < inputFilePaths.size( ); ++stringIndex ) {
        std::ifstream sourceFileStream( inputFilePaths[ stringIndex ].c_str( ), std::ios::in );

        std::string outputFilePath = mOutputPath;
        Types::StringVector inputFilePathComponents = Strings::Get( )->StringToWords( inputFilePaths[ stringIndex ], Constants::cFolderDelimiter );

        std::string fileName = inputFilePathComponents[ inputFilePathComponents.size( ) - 1 ];
        outputFilePath.append( fileName );

        std::ofstream destinationFileStream( outputFilePath.c_str( ), std::ios::out );
        if( destinationFileStream.is_open( ) ) {
            destinationFileStream << sourceFileStream.rdbuf( );
            sourceFileStream.close( );
            destinationFileStream.close( );
        } else {
            return false;
        }
    }
    return true;
}

void FileWriter::WriteOutputData( Types::EnvironmentPointer environment ) {
    bool success = false;

    if( WriteVectorDatums( ) )
        if( WriteMatrixDatums( ) )
            if( WriteTagData( environment->GetHeterotrophs( )->GetTagger( ) ) )
                if( WriteStateFile( environment ) )
                    success = true;

    if( success )
        std::cout << "Output data written to \"" << mOutputPath << "\"." << std::endl;
    else {
        std::cout << "ERROR> File writing failed. Could not access \"" << mOutputPath << "\"." << std::endl;
        exit( 1 );
    }
}

bool FileWriter::WriteVectorDatums( ) {
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
            return false;
        }
    }
    return true;
}

bool FileWriter::WriteMatrixDatums( ) {
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
            return false;
    }
    return true;
}

bool FileWriter::WriteTagData( Types::TaggerPointer tagger ) {

    unsigned numberOfTags = tagger->GetNumberOfTags( );

    if( numberOfTags > 0 ) {
        for( unsigned int tagIndex = 0; tagIndex < numberOfTags; ++tagIndex ) {
            Types::DataTagPointer tag = tagger->GetTag( tagIndex );

            std::string outputSubdirectory = mOutputPath;
            outputSubdirectory.append( Constants::cTagFileName + Strings::Get( )->ToString( tag->GetID( ) ) );
            mkdir( outputSubdirectory.c_str( ), Constants::cOutputFolderPermissions );
            outputSubdirectory.append( Strings::Get( )->ToString( Constants::cFolderDelimiter ) );

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
                return false;

            // Write tag time series
            Types::FloatVectorMap timeSeriesDataMap = tag->GetTimeSeriesData( );

            for( Types::FloatVectorMap::iterator iter = timeSeriesDataMap.begin( ); iter != timeSeriesDataMap.end( ); ++iter ) {
                std::string fileName = outputSubdirectory;

                fileName.append( iter->first );
                fileName.append( Constants::cFileNameExtension );
                std::ofstream outputFileStream;
                outputFileStream.open( fileName.c_str( ), std::ios::out );

                if( outputFileStream.is_open( ) == true ) {
                    Types::FloatVector data = iter->second;

                    for( unsigned index = 0; index < data.size( ) - 1; ++index ) {
                        outputFileStream << data[ index ] << Constants::cDataDelimiterValue;
                    }
                    outputFileStream << data[ data.size( ) - 1 ] << std::endl;
                    outputFileStream.close( );
                } else
                    return false;
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
                    return false;
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
                    return false;
            }
        }
    }
    return true;
}

bool FileWriter::WriteStateFile( Types::EnvironmentPointer environment ) {
    if( Parameters::Get( )->GetWriteModelState( ) == true ) {

        std::string fileName = mOutputPath;
        fileName.append( Constants::cModelStateFileName );

        std::ofstream modelStateFileStream;
        modelStateFileStream.open( fileName.c_str( ), std::ios::out );

        modelStateFileStream.flags( std::ios::scientific );
        modelStateFileStream.precision( std::numeric_limits< double >::digits10 );

        if( modelStateFileStream.is_open( ) == true ) {
            // Header (for consistency with general file reading function)
            modelStateFileStream << Constants::cModelStateFileName << std::endl;
            // Model variables
            modelStateFileStream << environment->GetNutrient( )->GetVolume( ) << std::endl;
            modelStateFileStream << environment->GetAutotrophs( )->GetVolume( ) << std::endl;

            Types::HeterotrophsPointer heterotrophs = environment->GetHeterotrophs( );

            for( unsigned populationIndex = 0; populationIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++populationIndex ) {
                for( unsigned individualIndex = 0; individualIndex < heterotrophs->GetSizeClassPopulation( populationIndex ); ++individualIndex ) {
                    Types::IndividualPointer individual = heterotrophs->GetIndividual( populationIndex, individualIndex );
                    modelStateFileStream << individual->GetSizeClassIndex( ) << Constants::cDataDelimiterValue << individual->GetHeritableTraits( )->GetValue( Constants::eVolume ) << Constants::cDataDelimiterValue << individual->GetVolumeActual( ) << std::endl;
                }
            }
            modelStateFileStream.close( );
        } else
            return false;
    }
    return true;
}
