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
#include "Time.h"

int main( ) {
    Logger::Get( )->LogMessage( Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up..." );
    Logger::Get( )->LogMessage( "" );

    FileReader fileReader;

    if( fileReader.ReadInputFiles( ) == true ) {
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
                Logger::Get( )->LogMessage( "t = " + Convertor::Get( )->ToString( Time::Get( )->GetTimeStep( ) ) + Constants::cDataDelimiterValue + Constants::cWhiteSpaceCharacter + Convertor::Get( )->ToString( percentCount ) + "% completed." );
                percentCount += 10;
            }

            // Data collection
            if( Time::Get( )->DoRecordData( ) == true ) {
                DataRecorder::Get( )->AddDataTo( "AxisTimeSteps", Time::Get( )->GetTimeStep( ) );
                DataRecorder::Get( )->AddDataTo( "SamplingTime", timer.Split( ) );
                isAlive = environment->RecordData( );
            }
            Time::Get( )->IncrementTimeStep( );

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
