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
            success = WriteStateFile( environment );

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
        unsigned datumSize = vectorDatum->GetSize( );
        if( datumSize > 0 ) {
            fileName.insert( 0, mOutputPath ).append( Constants::cFileNameExtension );
            std::ofstream outputFileStream;
            outputFileStream.open( fileName.c_str( ), std::ios::out );

            if( outputFileStream.is_open( ) == true ) {
                for( unsigned dataIndex = 0; dataIndex < datumSize - 1; ++dataIndex ) {
                    outputFileStream << vectorDatum->GetDataAtIndex( dataIndex ) << Constants::cDataDelimiterValue;
                }
                outputFileStream << vectorDatum->GetDataAtIndex( vectorDatum->GetSize( ) - 1 );
                outputFileStream.close( );
            } else {
                return false;
            }
        }
    }
    return true;
}

bool FileWriter::WriteMatrixDatums( ) {
    Types::MatrixDatumMap matrixDatumMap = DataRecorder::Get( )->GetMatrixDatumMap( );
    for( Types::MatrixDatumMap::iterator iter = matrixDatumMap.begin( ); iter != matrixDatumMap.end( ); ++iter ) {

        std::string fileName = iter->first;
        Types::MatrixDatumPointer matrixDatum = iter->second;
        unsigned rowSize = matrixDatum->GetRows( );
        if( rowSize > 0 ) {
            fileName.insert( 0, mOutputPath ).append( Constants::cFileNameExtension );
            std::ofstream outputFileStream;
            outputFileStream.open( fileName.c_str( ), std::ios::out );
            if( outputFileStream.is_open( ) == true ) {
                for( unsigned rowIndex = 0; rowIndex < rowSize; ++rowIndex ) {
                    for( unsigned columnIndex = 0; columnIndex < matrixDatum->GetColumns( ) - 1; ++columnIndex ) {
                        outputFileStream << matrixDatum->GetDataAtIndices( rowIndex, columnIndex ) << Constants::cDataDelimiterValue;
                    }
                    outputFileStream << matrixDatum->GetDataAtIndices( rowIndex, matrixDatum->GetColumns( ) - 1 ) << std::endl;
                }
                outputFileStream.close( );
            } else
                return false;
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

            for( unsigned populationIndex = 0; populationIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++populationIndex ) {
                for( unsigned individualIndex = 0; individualIndex < environment->GetHeterotrophs( )->GetSizeClassPopulation( populationIndex ); ++individualIndex ) {
                    Types::IndividualPointer individual = environment->GetHeterotrophs( )->GetIndividual( populationIndex, individualIndex );
                    modelStateFileStream << individual->GetSizeClassIndex( ) << Constants::cDataDelimiterValue << individual->GetHeritableTraits( )->GetValue( Constants::eVolume ) << Constants::cDataDelimiterValue << individual->GetVolumeActual( ) << std::endl;
                }
            }
            modelStateFileStream.close( );
        } else
            return false;
    }
    return true;
}
