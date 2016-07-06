#include "FileWriter.h"

#include "DataRecorder.h"
#include "Convertor.h"
#include "IntegerVectorDatum.h"
#include "IntegerMatrixDatum.h"
#include "FloatVectorDatum.h"
#include "FloatMatrixDatum.h"
#include "Parameters.h"
#include "DateTime.h"
#include "Logger.h"
#include "Environment.h"
#include "Nutrient.h"
#include "Phytoplankton.h"
#include "HeterotrophData.h"
#include "Heterotrophs.h"
#include "Individual.h"
#include "Genome.h"

#include <sys/stat.h> // For mkdir

Types::FileWriterPointer FileWriter::mThis = 0;

Types::FileWriterPointer FileWriter::Get( ) {
    if( mThis == 0 ) {
        mThis = new FileWriter( );
    }

    return mThis;
}

FileWriter::FileWriter( ) {
    mWidthOfWidestParameterSet = 0;
    mDataSetDirectoryNameIsSet = false;
}

FileWriter::~FileWriter( ) {
    mParameterValues.clear( );
    mParameterNames.clear( );
}

void FileWriter::InitialiseOutputPath( ) {

    // output/
    mOutputPath = Constants::cOutputDirectoryName;
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Constants::cFolderDelimiter );

    // output/[version]/
    mOutputPath.append( Constants::cSystemVersion );
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Constants::cFolderDelimiter );

    // output/[version]/[experiment]/
    mOutputPath.append( Parameters::Get( )->GetExperimentName( ) );
    mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
    mOutputPath.append( Constants::cFolderDelimiter );

    // output/[version]/[experiment]/[date and time]
    if( mDataSetDirectoryNameIsSet == false ) {
        mDataSetDirectoryName = DateTime::Get( )->GetFormattedDateAndTime( Constants::cDataSetNameFormat, false );
    }

    mOutputPath.append( mDataSetDirectoryName );
    int returnValue = mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );

    // The following code ensures the date are written into a unique subdirectory.
    if( returnValue == -1 ) {
        mOutputPath.append( "_" );
        int stringLength = mOutputPath.length( );

        int count = 1;
        while( returnValue == -1 ) {
            mOutputPath.replace( stringLength, 1, Convertor::Get( )->NumberToString( count ) );
            returnValue = mkdir( mOutputPath.c_str( ), Constants::cOutputFolderPermissions );
            ++count;
        }
    }

    mOutputPath.append( Constants::cFolderDelimiter );

    // Write constants file

    CreateParameterStringVectors( );

    std::string fileName;

    fileName = mOutputPath;
    fileName.append( Constants::cConstantsFileName );
    fileName.append( Constants::cTextFileExtension );

    Types::OutputFileStream constantsFile;
    constantsFile.open( fileName.c_str( ) );

    constantsFile << Constants::cSystemName << " " << Constants::cSystemVersion << " run details" << std::endl;

    for( unsigned int lineNumber = 0; lineNumber < mParameterNames.size( ); ++lineNumber ) {
        constantsFile << GetParameterLine( lineNumber ) << std::endl;
    }

    constantsFile.close( );

    mTerminalFilePath = mOutputPath;
    mTerminalFilePath.append( "Terminal" );
    mTerminalFilePath.append( Constants::cTextFileExtension );

    Logger::Get( )->LogString( "Data output directory is \"" + mOutputPath + "\"." );
    Logger::Get( )->LogString( "" );
}

void FileWriter::WriteOutputDataToFile( Types::EnvironmentPointer environment, const unsigned int totalNumberOfTimeSteps, const unsigned int runTimeInSeconds, const bool didHeterotrophsLive ) {

    std::string fileName;

    // Write axis vector names metadata file

    fileName = Constants::cAxisVectorNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream axisVectorNamesFile;
    axisVectorNamesFile.open( fileName.c_str( ) );

    for( unsigned int axisVectorIndex = 0; axisVectorIndex < Constants::cNumberOfAxisVectors; ++axisVectorIndex ) {
        axisVectorNamesFile << Constants::cAxisVectorNames[ axisVectorIndex ] << Constants::cWordDelimiter;
    }
    axisVectorNamesFile.close( );

    // Write trophic matrix names metadata file

    fileName = Constants::cTrophicDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream trophicDatumNamesFile;
    trophicDatumNamesFile.open( fileName.c_str( ) );

    for( unsigned int trophicDatumIndex = 0; trophicDatumIndex < Constants::cNumberOfTrophicDatums; ++trophicDatumIndex ) {
        trophicDatumNamesFile << Constants::cTrophicDatumNames[ trophicDatumIndex ] << Constants::cWordDelimiter;
    }
    trophicDatumNamesFile.close( );

    // Prepare vector datum names metadata file stream

    fileName = Constants::cVectorDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream vectorDatumNamesFile;
    vectorDatumNamesFile.open( fileName.c_str( ) );

    // Prepare matrix datum names metadata file stream

    fileName = Constants::cMatrixDatumNamesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream matrixDatumNamesFile;
    matrixDatumNamesFile.open( fileName.c_str( ) );

    // Prepare vector enum index metadata file stream

    fileName = Constants::cVectorEnumIndicesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream vectorEnumIndiciesFile;
    vectorEnumIndiciesFile.open( fileName.c_str( ) );

    // Prepare matrix enum index metadata file stream

    fileName = Constants::cMatrixEnumIndicesFileName;
    fileName.insert( 0, mOutputPath ).append( Constants::cOutputFileExtension );
    Types::OutputFileStream matrixEnumIndiciesFile;
    matrixEnumIndiciesFile.open( fileName.c_str( ) );

    // Write datum files

    for( unsigned int datumIndex = 0; datumIndex < DataRecorder::Get( )->GetNumberOfDatums( ); ++datumIndex ) {

        fileName = mOutputPath;
        std::string datumName = DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetName( );
        fileName.append( datumName.append( Constants::cOutputFileExtension ) );

        Types::OutputFileStream datumFile;
        datumFile.open( fileName.c_str( ), Types::OutputFileStream::app );
        
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
                datumFile << integerVectorDatumPointer->GetData( index ) << Constants::cOutputValueDelimiter;
            }

        } else if( DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex )->GetType( ) == Constants::eFloatVector ) {

            Types::FloatVectorDatumPointer floatVectorDatumPointer = ( Types::FloatVectorDatumPointer )DataRecorder::Get( )->GetDatumPointerFromIndex( datumIndex );

            // Do not write metadata for axis vector files or ad-hoc vectors.
            if( IsNameOfVectorDatum( floatVectorDatumPointer->GetName( ) ) == true ) {
                vectorDatumNamesFile << floatVectorDatumPointer->GetName( ) << Constants::cWordDelimiter;
                vectorEnumIndiciesFile << floatVectorDatumPointer->GetEnumIndex( ) << Constants::cWordDelimiter;
            }

            int datumLength = floatVectorDatumPointer->GetLength( );
            mNumberOfDataPoints = datumLength;

            for( int index = 0; index < datumLength; ++index ) {
                datumFile << floatVectorDatumPointer->GetData( index ) << Constants::cOutputValueDelimiter;
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
                    datumFile << integerMatrixDatumPointer->GetData( row, column ) << Constants::cOutputValueDelimiter;
                }
                datumFile << integerMatrixDatumPointer->GetData( row, columns - 1 ) << Constants::cOutputValueDelimiter << std::endl;
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
                    datumFile << floatMatrixDatumPointer->GetData( row, column ) << Constants::cOutputValueDelimiter;
                }
                datumFile << floatMatrixDatumPointer->GetData( row, columns - 1 ) << Constants::cOutputValueDelimiter << std::endl;
            }
        }

        datumFile.close( );
    }

    vectorDatumNamesFile.close( );
    matrixDatumNamesFile.close( );
    vectorEnumIndiciesFile.close( );
    matrixEnumIndiciesFile.close( );

    // Finish Writing constants file

    CreateParameterStringVectors( totalNumberOfTimeSteps, runTimeInSeconds, didHeterotrophsLive );

    fileName = mOutputPath;
    fileName.append( Constants::cConstantsFileName );
    fileName.append( Constants::cTextFileExtension );

    Types::OutputFileStream constantsFile;
    constantsFile.open( fileName.c_str( ), Types::OutputFileStream::app );

    for( unsigned int lineNumber = 0; lineNumber < mParameterNames.size( ); ++lineNumber ) {
        constantsFile << GetParameterLine( lineNumber ) << std::endl;
    }

    constantsFile.close( );

    // Write model end state to file

    fileName = mOutputPath;
    fileName.append( Constants::cStateFileName );
    fileName.append( Constants::cTextFileExtension );

    Types::OutputFileStream stateFile;
    stateFile.open( fileName.c_str( ) );

    stateFile << totalNumberOfTimeSteps << std::endl;
    stateFile << environment->GetNutrient( )->GetVolume( ) << std::endl;
    stateFile << environment->GetPhytoplankton( )->GetVolume( ) << std::endl;

    Types::HeterotrophsPointer heterotrophs = environment->GetHeterotrophs( );

    for( unsigned int sizeClassIndex = 0; sizeClassIndex < Parameters::Get( )->GetNumberOfSizeClasses( ); ++sizeClassIndex ) {
        for( unsigned int individualIndex = 0; individualIndex < heterotrophs->GetSizeClassSize( sizeClassIndex ); ++individualIndex ) {

            Types::IndividualPointer individual = heterotrophs->GetIndividual( sizeClassIndex, individualIndex );
            stateFile << individual->GetGenome( )->GetGeneValue( Constants::eVolumeGene ) << Constants::cOutputValueDelimiter << individual->GetVolumeActual( ) << Constants::cOutputValueDelimiter << individual->GetTrophicLevel( ) << Constants::cOutputValueDelimiter << individual->GetSizeClassIndex( ) << Constants::cOutputValueDelimiter << individual->GetAge( ) << std::endl;
        }
    }

    stateFile.close( );
}

const std::string FileWriter::GetOutputPath( ) {
    return mOutputPath;
}

void FileWriter::SetDataSetDirectoryName( const std::string dataSetDirectoryName ) {
    mDataSetDirectoryName = dataSetDirectoryName;
    mDataSetDirectoryNameIsSet = true;
}

void FileWriter::LogTerminalToFile( const std::string logString ) {
    mTerminalFile.open( mTerminalFilePath.c_str( ), Types::OutputFileStream::app );
    mTerminalFile << logString;
    mTerminalFile.close( );
}

void FileWriter::CreateParameterStringVectors( const unsigned int totalNumberOfTimeSteps, const unsigned int runTimeInSeconds, const bool didHeterotrophsLive ) {

    if( totalNumberOfTimeSteps == 0 ) {

        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "ExperimentName" );
        mParameterValues.push_back( Parameters::Get( )->GetExperimentName( ) );
        mParameterNames.push_back( "RandomNumberSeed" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetRandomNumberSeed( ) ) );
        mParameterNames.push_back( "RunTimeInSeconds" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetRunTimeInSeconds( ) ) );
        mParameterNames.push_back( "SamplingRate" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetSamplingRate( ) ) );
        mParameterNames.push_back( "NumberOfSizeClasses" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetNumberOfSizeClasses( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "HeterotrophInitialisationMethod" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetHeterotrophInitialisationMethod( ) ) );
        mParameterNames.push_back( "PreferenceFunctionType" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetPreferenceFunctionType( ) ) );
        mParameterNames.push_back( "FunctionalResponseType" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetFunctionalResponseType( ) ) );
        mParameterNames.push_back( "StarvationFunctionType" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetStarvationFunctionType( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "InitialNutrientVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetInitialNutrientVolume( ) ) );
        mParameterNames.push_back( "InitialPhytoplanktonVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetInitialPhytoplanktonVolume( ) ) );
        mParameterNames.push_back( "InitialHeterotrophVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetInitialHeterotrophVolume( ) ) );
        mParameterNames.push_back( "MinimumHeterotrophVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetMinimumHeterotrophVolume( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "SmallestIndividualVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetSmallestIndividualVolume( ) ) );
        mParameterNames.push_back( "LargestIndividualVolume" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetLargestIndividualVolume( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "PreferredPreyVolumeRatio" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetPreferredPreyVolumeRatio( ) ) );
        mParameterNames.push_back( "PreferenceFunctionWidth" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetPreferenceFunctionWidth( ) ) );
        mParameterNames.push_back( "PreferenceFunctionHeight" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetPreferenceFunctionHeight( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "SizeClassSubsetFraction" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetSizeClassSubsetFraction( ) ) );
        mParameterNames.push_back( "HalfSaturationConstantFraction" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetHalfSaturationConstantFraction( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "AssimilationEfficiency" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetAssimilationEfficiency( ) ) );
        mParameterNames.push_back( "FractionalMetabolicExpensePerTimeStep" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetFractionalMetabolicExpensePerTimeStep( ) ) );
        mParameterNames.push_back( "MetabolicIndex" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetMetabolicIndex( ) ) );
        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "MutationProbability" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetMutationProbability( ) ) );
        mParameterNames.push_back( "MutationStandardDeviation" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( Parameters::Get( )->GetMutationStandardDeviation( ) ) );

        for( unsigned int parameterSetIndex = 0; parameterSetIndex < mParameterNames.size( ); ++parameterSetIndex ) {

            unsigned int widthOfParameterSet = mParameterNames[ parameterSetIndex ].size( ) + mParameterValues[ parameterSetIndex ].size( );

            if( widthOfParameterSet > mWidthOfWidestParameterSet ) {

                mWidthOfWidestParameterSet = widthOfParameterSet;
            }
        }

        mWidthOfWidestParameterSet = mWidthOfWidestParameterSet + Constants::cMinimumParameterNameValueSpacing + Constants::cParameterNameValueDelimiter.size( );

    } else {

        mParameterNames.clear( );
        mParameterValues.clear( );

        mParameterNames.push_back( "-" );
        mParameterValues.push_back( "-" );
        mParameterNames.push_back( "Completed" );
        mParameterValues.push_back( DateTime::Get( )->GetFormattedDateAndTime( Constants::cCompleteDateFormat ) );
        mParameterNames.push_back( "TotalNumberOfTimeSteps" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( totalNumberOfTimeSteps ) );
        mParameterNames.push_back( "DataPointsCollected" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( mNumberOfDataPoints ) );
        mParameterNames.push_back( "ActualRunTimeInSeconds" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( runTimeInSeconds ) );
        mParameterNames.push_back( "HeterotrophsLived" );
        mParameterValues.push_back( Convertor::Get( )->NumberToString( didHeterotrophsLive ) );
        mParameterNames.push_back( "RandomSeedValue" );
        int seed = 0;
        
        if( Parameters::Get( )->GetRandomNumberSeed( ) == 1 ) {
            seed = DateTime::Get( )->GetRandomNumberSeed( );
        } 
        mParameterValues.push_back( Convertor::Get( )->NumberToString( seed ) );
        mParameterNames.push_back( "DataSetName" );
        mParameterValues.push_back( DateTime::Get( )->GetFormattedDateAndTime( Constants::cDataSetNameFormat, false ) );
    }
}

const std::string FileWriter::GetParameterLine( const int lineNumber ) const {
    
    std::string parameterLine;

    if( mParameterNames[ lineNumber ] == "-" ) {

        parameterLine.append( mWidthOfWidestParameterSet, '-' );

    } else {
        unsigned int numberOfSpaces = mWidthOfWidestParameterSet - ( mParameterNames[ lineNumber ].size( ) + Constants::cParameterNameValueDelimiter.size( ) + mParameterValues[ lineNumber ].size( ) );

        parameterLine = mParameterNames[ lineNumber ];
        parameterLine.append( numberOfSpaces, Constants::cParameterNameValueSpacer );
        parameterLine.append( Constants::cParameterNameValueDelimiter );
        parameterLine.append( mParameterValues[ lineNumber ] );
    }

    return parameterLine;
}

bool FileWriter::IsNameOfVectorDatum( const std::string datumName ) const {

    bool isVectorDatum = false;

    for( unsigned int vectorDatumIndex = 0; vectorDatumIndex < Constants::cNumberOfVectorDatums; ++vectorDatumIndex ) {

        if( datumName == Constants::cVectorDatumNames[ vectorDatumIndex ] ) {
            isVectorDatum = true;

            break;
        }
    }

    return isVectorDatum;
}

bool FileWriter::IsNameOfSizeClassDatum( const std::string datumName ) const {

    bool isSizeClassDatum = false;

    for( unsigned int sizeClassDatumIndex = 0; sizeClassDatumIndex < Constants::cNumberOfSizeClassDatums; ++sizeClassDatumIndex ) {

        if( datumName == Constants::cSizeClassDatumNames[ sizeClassDatumIndex ] ) {
            isSizeClassDatum = true;

            break;
        }
    }

    return isSizeClassDatum;
}
