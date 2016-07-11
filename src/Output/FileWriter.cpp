#include "FileWriter.h"

#include "DataRecorder.h"
#include "Convertor.h"
#include "IntegerVectorDatum.h"
#include "IntegerMatrixDatum.h"
#include "FloatVectorDatum.h"
#include "FloatMatrixDatum.h"
#include "Parameters.h"
#include "Logger.h"
#include "Environment.h"
#include "Nutrient.h"
#include "Autotroph.h"
#include "HeterotrophData.h"
#include "Heterotrophs.h"
#include "Individual.h"
#include "Genome.h"
#include "Date.h"

FileWriter::FileWriter( ) {
}

FileWriter::~FileWriter( ) {

}

void FileWriter::WriteFiles( ) {
    InitialiseOutputDirectory( );
    WriteOutputData( );
    WriteInputFiles( );
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

    // The following code ensures the date are written into a unique subdirectory.
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

void FileWriter::WriteOutputData( ) {
    std::string fileName;

    // Write axis vector names metadata file
    fileName = Constants::cAxisVectorNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream axisVectorNamesFile;
    axisVectorNamesFile.open( fileName.c_str( ) );

    for( unsigned axisVectorIndex = 0; axisVectorIndex < Constants::cNumberOfAxisVectors; ++axisVectorIndex ) {
        axisVectorNamesFile << Constants::cAxisVectorNames[ axisVectorIndex ] << Constants::cWordDelimiter;
    }
    axisVectorNamesFile.close( );

    // Write trophic matrix names metadata file
    fileName = Constants::cTrophicDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream trophicDatumNamesFile;
    trophicDatumNamesFile.open( fileName.c_str( ) );

    for( unsigned trophicDatumIndex = 0; trophicDatumIndex < Constants::cNumberOfTrophicDatums; ++trophicDatumIndex ) {
        trophicDatumNamesFile << Constants::cTrophicDatumNames[ trophicDatumIndex ] << Constants::cWordDelimiter;
    }
    trophicDatumNamesFile.close( );

    // Prepare vector datum names metadata file stream
    fileName = Constants::cVectorDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream vectorDatumNamesFile;
    vectorDatumNamesFile.open( fileName.c_str( ) );

    // Prepare matrix datum names metadata file stream
    fileName = Constants::cMatrixDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream matrixDatumNamesFile;
    matrixDatumNamesFile.open( fileName.c_str( ) );

    // Prepare vector enum index metadata file stream
    fileName = Constants::cVectorEnumIndicesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream vectorEnumIndiciesFile;
    vectorEnumIndiciesFile.open( fileName.c_str( ) );

    // Prepare matrix enum index metadata file stream
    fileName = Constants::cMatrixEnumIndicesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    std::ofstream matrixEnumIndiciesFile;
    matrixEnumIndiciesFile.open( fileName.c_str( ) );

    // Write datum files
    for( unsigned datumIndex = 0; datumIndex < DataRecorder::Get( )->GetNumberOfDatums( ); ++datumIndex ) {

        fileName = mOutputPath;
        std::string datumName = DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetName( );
        fileName.append( datumName.append( Constants::cOutputFileExtension ) );

        std::ofstream datumFile;
        datumFile.open( fileName.c_str( ), std::ofstream::app );

        // FIX - Replace nested if with a switch statement.
        if( DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetType( ) == Constants::eIntegerVector ) {

            Types::IntegerVectorDatumPointer integerVectorDatumPointer = ( Types::IntegerVectorDatumPointer )DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex );

            // Do not write metadata for axis vector files or ad-hoc vectors.
            if( IsNameOfVectorDatum( integerVectorDatumPointer->GetName( ) ) == true ) {
                vectorDatumNamesFile << integerVectorDatumPointer->GetName( ) << Constants::cWordDelimiter;
                vectorEnumIndiciesFile << integerVectorDatumPointer->GetEnumIndex( ) << Constants::cWordDelimiter;
            }

            int datumLength = integerVectorDatumPointer->GetLength( );

            for( int index = 0; index < datumLength; ++index ) {
                datumFile << integerVectorDatumPointer->GetData( index ) << Constants::cDataDelimiterValue;
            }

        } else if( DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetType( ) == Constants::eFloatVector ) {

            Types::FloatVectorDatumPointer floatVectorDatumPointer = ( Types::FloatVectorDatumPointer )DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex );

            // Do not write metadata for axis vector files or ad-hoc vectors.
            if( IsNameOfVectorDatum( floatVectorDatumPointer->GetName( ) ) == true ) {
                vectorDatumNamesFile << floatVectorDatumPointer->GetName( ) << Constants::cWordDelimiter;
                vectorEnumIndiciesFile << floatVectorDatumPointer->GetEnumIndex( ) << Constants::cWordDelimiter;
            }

            int datumLength = floatVectorDatumPointer->GetLength( );

            for( int index = 0; index < datumLength; ++index ) {
                datumFile << floatVectorDatumPointer->GetData( index ) << Constants::cDataDelimiterValue;
            }

        } else if( DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetType( ) == Constants::eIntegerMatrix ) {

            Types::IntegerMatrixDatumPointer integerMatrixDatumPointer = ( Types::IntegerMatrixDatumPointer )DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex );

            // Do not write metadata for ad-hoc matrices.
            if( IsNameOfSizeClassDatum( integerMatrixDatumPointer->GetName( ) ) == true ) {
                matrixDatumNamesFile << integerMatrixDatumPointer->GetName( ) << Constants::cWordDelimiter;
                matrixEnumIndiciesFile << integerMatrixDatumPointer->GetEnumIndex( ) << Constants::cWordDelimiter;
            }

            int rows = integerMatrixDatumPointer->GetRows( );
            int columns = integerMatrixDatumPointer->GetColumns( );

            for( int row = 0; row < rows; ++row ) {
                for( int column = 0; column < columns - 1; ++column ) {
                    datumFile << integerMatrixDatumPointer->GetData( row, column ) << Constants::cDataDelimiterValue;
                }
                datumFile << integerMatrixDatumPointer->GetData( row, columns - 1 ) << Constants::cDataDelimiterValue << std::endl;
            }

        } else if( DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetType( ) == Constants::eFloatMatrix ) {

            Types::FloatMatrixDatumPointer floatMatrixDatumPointer = ( Types::FloatMatrixDatumPointer )DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex );

            // Do not write metadata for ad-hoc matrices.
            if( IsNameOfSizeClassDatum( floatMatrixDatumPointer->GetName( ) ) == true ) {
                matrixDatumNamesFile << floatMatrixDatumPointer->GetName( ) << Constants::cWordDelimiter;
                matrixEnumIndiciesFile << floatMatrixDatumPointer->GetEnumIndex( ) << Constants::cWordDelimiter;
            }

            int rows = floatMatrixDatumPointer->GetRows( );
            int columns = floatMatrixDatumPointer->GetColumns( );

            for( int row = 0; row < rows; ++row ) {
                for( int column = 0; column < columns - 1; ++column ) {
                    datumFile << floatMatrixDatumPointer->GetData( row, column ) << Constants::cDataDelimiterValue;
                }
                datumFile << floatMatrixDatumPointer->GetData( row, columns - 1 ) << Constants::cDataDelimiterValue << std::endl;
            }
        }
        datumFile.close( );
    }
    vectorDatumNamesFile.close( );
    matrixDatumNamesFile.close( );
    vectorEnumIndiciesFile.close( );
    matrixEnumIndiciesFile.close( );
    
    Logger::Get( )->LogMessage( "Output data written to \"" + mOutputPath + "\"." );
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

const std::string FileWriter::GetOutputPath( ) {
    return mOutputPath;
}

bool FileWriter::IsNameOfVectorDatum( const std::string datumName ) const {
    bool isVectorDatum = false;

    for( unsigned vectorDatumIndex = 0; vectorDatumIndex < Constants::cNumberOfVectorDatums; ++vectorDatumIndex ) {
        if( datumName == Constants::cVectorDatumNames[ vectorDatumIndex ] ) {
            isVectorDatum = true;
            break;
        }
    }
    return isVectorDatum;
}

bool FileWriter::IsNameOfSizeClassDatum( const std::string datumName ) const {
    bool isSizeClassDatum = false;

    for( unsigned sizeClassDatumIndex = 0; sizeClassDatumIndex < Constants::cNumberOfSizeClassDatums; ++sizeClassDatumIndex ) {
        if( datumName == Constants::cSizeClassDatumNames[ sizeClassDatumIndex ] ) {
            isSizeClassDatum = true;
            break;
        }
    }
    return isSizeClassDatum;
}
