#include "Logger.h"
#include "Constants.h"
#include "Parameters.h"
#include "FileReader.h"
#include "Convertor.h"
#include "Environment.h"
#include "DataRecorder.h"
#include "FileWriter.h"
#include "Timer.h"
#include "HeterotrophProcessor.h"
#include "TimeStep.h"

#include "RandomSFMT.h"

int main( int numberOfArguments, char* commandlineArguments[ ] ) {
    Logger::Get( )->LogMessage( Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up..." );
    Logger::Get( )->LogMessage( "" );

    FileReader fileReader;

    std::string stateFile = "";
    if( numberOfArguments > 1 ) stateFile = commandlineArguments[ 1 ];

    if( fileReader.ReadInputFiles( stateFile ) == true ) {
        Timer timer = Timer( true );
        FileWriter fileWriter;
        Types::EnvironmentPointer environment = new Environment( );

        double oneTenthOfRunTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( ) / 10.0;
        double cumulativeTenthsOfRunTime = 0;
        unsigned percentCount = 0;
        bool isAlive = true;

        Logger::Get( )->LogMessage( "" );
        Logger::Get( )->LogMessage( "Starting main time loop..." );
        do {
            // Update before data collection; calculates essential variables for encounter rates.
            environment->Update( );

            // Text output at the completion of each ten percent of the run 
            if( timer.Elapsed( ) >= ( unsigned )cumulativeTenthsOfRunTime ) {
                cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
                Logger::Get( )->LogMessage( "t = " + Convertor::Get( )->ToString( TimeStep::Get( )->GetTimeStep( ) ) + Constants::cDataDelimiterValue + Constants::cWhiteSpaceCharacter + Convertor::Get( )->ToString( percentCount ) + "% completed." );
                percentCount += 10;
            }

            // Data collection
            if( TimeStep::Get( )->DoRecordData( ) == true ) {
                DataRecorder::Get( )->AddDataTo( "AxisTimeSteps", TimeStep::Get( )->GetTimeStep( ) );
                DataRecorder::Get( )->AddDataTo( "SamplingTime", timer.Split( ) );
                isAlive = environment->RecordData( );
            }
            TimeStep::Get( )->IncrementTimeStep( );

        } while( timer.Elapsed( ) < Parameters::Get( )->GetRunTimeInSeconds( ) && isAlive == true );
        if( timer.Elapsed( ) >= Parameters::Get( )->GetRunTimeInSeconds( ) ) {
            Logger::Get( )->LogMessage( "Main time loop complete." );
        } else {
            Logger::Get( )->LogMessage( "Heterotroph population crashed. Main time loop aborted." );
        }
        Logger::Get( )->LogMessage( "" );

        fileWriter.WriteOutputData( environment );
        Logger::Get( )->LogMessage( "Total run time " + Convertor::Get( )->ToString( timer.Stop( ) ) + "s" );

        delete environment;
    } else {
        Logger::Get( )->LogMessage( "ERROR> File reading failed. System exiting..." );
    }

    return 0;
}
