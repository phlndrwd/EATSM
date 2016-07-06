#include "Logger.h"
#include "Constants.h"
#include "Parameters.h"
#include "FileReader.h"
#include "DateTime.h"
#include "Convertor.h"
#include "Environment.h"
#include "DataRecorder.h"
#include "FileWriter.h"
#include "HeterotrophProcessor.h"
#include "Individual.h"
#include "RestartState.h"
#include "Heterotrophs.h"

Types::FileReaderPointer mFileReader;

void RunModel( ) {
    
    DateTime::Get( )->StartTiming( );
    FileWriter::Get( )->InitialiseOutputPath( );

    Types::EnvironmentPointer environment = new Environment( );

    Logger::Get( )->LogString( "" );

    unsigned int runTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( );

    double oneTenthOfRunTimeInSeconds = runTimeInSeconds / 10.0;
    unsigned int percentCount = 0;

    unsigned int cumulativeTimeInSeconds;
    unsigned int timeStep = 0;
    double cumulativeTenthsOfRunTime = 0;

    bool isAlive = true;

    if( RestartState::Get( )->IsRestart( ) == true ) {
        timeStep = RestartState::Get( )->GetStartingTimeStep( );
        Logger::Get( )->LogString( "Restarting main time loop..." );
    } else {
        Logger::Get( )->LogString( "Starting main time loop..." );
    }

    do {
        DateTime::Get( )->SplitTiming( );
        cumulativeTimeInSeconds = DateTime::Get( )->GetCumulativeTimeInSeconds( );

        // Update before data collection; calculates essential variables for encounter rates.
        environment->Update( );

        // Text output for the completion of each ten percent of the run 
        if( cumulativeTimeInSeconds >= ( unsigned int )cumulativeTenthsOfRunTime ) {
            cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
            Logger::Get( )->LogString( "t = " + Convertor::Get( )->NumberToString( timeStep ) + Constants::cOutputValueDelimiter + Convertor::Get( )->NumberToString( percentCount ) + "% completed." );
            percentCount += 10;
        }

        // Data collection
        if( timeStep % Parameters::Get( )->GetSamplingRate( ) == 0 ) {
            DataRecorder::Get( )->AddIntegerVectorData( Constants::eAxisAbstractTime, Constants::cAxisVectorNames[ Constants::eAxisAbstractTime ], timeStep );
            isAlive = environment->RecordData( );
        }

        timeStep = timeStep + 1;
    } while( cumulativeTimeInSeconds < runTimeInSeconds && isAlive == true );

    if( cumulativeTimeInSeconds >= runTimeInSeconds ) {
        Logger::Get( )->LogString( "Main time loop complete." );
    } else {
        Logger::Get( )->LogError( "Heterotroph population crashed. Main time loop aborted." );
    }

    Logger::Get( )->LogString( "" );
    Logger::Get( )->OutputRunTime( );
    Logger::Get( )->LogString( DateTime::Get( )->GetFormattedDateAndTime( Constants::cCompleteDateFormat ) );
    Logger::Get( )->LogString( "" );
    Logger::Get( )->LogString( "Summary..." );
    environment->OutputHeterotrophSummaryData( );
    Logger::Get( )->LogString( "" );
    FileWriter::Get( )->WriteOutputDataToFile( environment, timeStep, cumulativeTimeInSeconds, isAlive );
    Logger::Get( )->LogString( "Output data written to file." );

    delete environment;
}

const std::string GetFileReadOutputMessage( const Constants::eReturnCodes returnCode ) {

    std::string outputMessage;

    switch( returnCode ) {

        case Constants::eIncorrectParameterPath:
            outputMessage = "Could not access \"" + mFileReader->GetParametersFilePath( ) + "\". System exiting.";
            break;

        case Constants::eIncorrectVersion:
            outputMessage = "Incorrect version \"" + mFileReader->GetParametersFilePath( ) + "\". System exiting.";
            break;

        default:
            // Do nothing
            break;
    }

    return outputMessage;
}

int main( int numberOfArguments, char* commandlineArguments[ ] ) {

    bool runModel = false;

    std::string startUpString = Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up...";

    switch( numberOfArguments ) {
        case 1:
        {
            Logger::Get( )->LogString( startUpString, true );
            Parameters::Get( )->SetConstants( );
            Parameters::Get( )->CalculateSizeClassVectors( );
            Parameters::Get( )->CalculateVariables( );
            Logger::Get( )->LogString( "Using default constant values.", true );

            runModel = true;

            break;
        }

        case 2:
        {
            std::string command = commandlineArguments[ 1 ];

            if( command == Constants::cVersionCommandLineArgument ) {

                Logger::Get( )->LogString( Constants::cSystemName );
                Logger::Get( )->LogString( Constants::cSystemVersion );
                Logger::Get( )->LogString( Constants::cSystemDate );
                Logger::Get( )->LogString( Constants::cSystemTime );

                runModel = false;

            } else {

                mFileReader = new FileReader( );

                Constants::eReturnCodes returnCode = mFileReader->ReadParametersFile( command );

                if( returnCode == Constants::eSuccessful ) {

                    Logger::Get( )->LogString( startUpString, true );
                    Parameters::Get( )->CalculateSizeClassVectors( );
                    Parameters::Get( )->CalculateVariables( );
                    Logger::Get( )->LogString( "Using constants input file \"" + mFileReader->GetParametersFilePath( ) + "\".", true );
                    runModel = true;
                } else {
                    Logger::Get( )->LogError( GetFileReadOutputMessage( returnCode ), true );
                }

                delete mFileReader;
            }

            break;
        }

        case 3:
        {
            std::string command = commandlineArguments[ 1 ];

            if( command == Constants::cRestartCommandLineArgument ) {

                mFileReader = new FileReader( );

                Constants::eReturnCodes returnCode = mFileReader->Restart( commandlineArguments[ 2 ] );

                if( returnCode == Constants::eSuccessful ) {

                    Logger::Get( )->LogString( startUpString, true );
                    // Calculate variables (necessarily) called in restart function.
                    Logger::Get( )->LogString( "Using restart input file \"" + mFileReader->GetParametersFilePath( ) + "\".", true );
                    runModel = true;
                } else {
                    Logger::Get( )->LogError( GetFileReadOutputMessage( returnCode ), true );
                }

                delete mFileReader;

            } else {
                Logger::Get( )->LogError( "Badly formed argument. System exiting.", true );
            }
            break;
        }

        case 5:
        {
            std::string command = commandlineArguments[ 1 ];

            if( command == Constants::cExperimentCommandLineArgument ) {

                std::string constantsFile = commandlineArguments[ 4 ];

                mFileReader = new FileReader( );

                Constants::eReturnCodes returnCode = mFileReader->ReadParametersFile( constantsFile );

                if( returnCode == Constants::eSuccessful ) {

                    std::string experimentName = commandlineArguments[ 2 ];
                    std::string dataSetDirectoryName = Convertor::Get( )->ParameterNamesListToValuesString( Convertor::Get( )->StringToWords( commandlineArguments[ 3 ], Constants::cStringSplitCharacter ) );

                    Logger::Get( )->LogString( startUpString, true );
                    Parameters::Get( )->CalculateSizeClassVectors( );
                    Parameters::Get( )->CalculateVariables( );
                    Parameters::Get( )->SetExperimentName( experimentName );

                    FileWriter::Get( )->SetDataSetDirectoryName( dataSetDirectoryName );

                    Logger::Get( )->LogString( "Using constants input file \"" + mFileReader->GetParametersFilePath( ) + "\".", true );
                    runModel = true;
                } else {
                    Logger::Get( )->LogError( GetFileReadOutputMessage( returnCode ), true );
                }

                delete mFileReader;

            }
            break;
        }

        default:
            Logger::Get( )->LogError( "Badly formed argument. System exiting.", true );
            break;
    }

    if( runModel == true ) {

        DataRecorder::Get( )->AddFloatVectorData( Constants::eAxisSizeClassBoundaryValues, Constants::cAxisVectorNames[ Constants::eAxisSizeClassBoundaryValues ], Parameters::Get( )->GetSizeClassBoundaries( ) );
        DataRecorder::Get( )->AddFloatVectorData( Constants::eAxisSizeClassMidPointValues, Constants::cAxisVectorNames[ Constants::eAxisSizeClassMidPointValues ], Parameters::Get( )->GetSizeClassMidPoints( ) );

        RunModel( );
    }

    return 0;
}
