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

int main( ) {
    Logger::Get( )->LogMessage( Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up..." );
    Logger::Get( )->LogMessage( "" );

    FileReader fileReader;
    fileReader.ReadInputFiles( );

    Types::EnvironmentPointer environment = new Environment( fileReader.GetRawTextData( ) );

    Timer timer;
    double oneTenthOfRunTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( ) / 10.0;
    double cumulativeTenthsOfRunTime = 0;
    unsigned cumulativeTimeInSeconds = 0;
    unsigned percentCount = 0;
    unsigned timeStep = 0;
    bool isAlive = true;
    
    Logger::Get( )->LogMessage( "" );
    Logger::Get( )->LogMessage( "Starting main time loop..." );
    timer.Start( );
    do {
        cumulativeTimeInSeconds = timer.Split( );

        // Update before data collection; calculates essential variables for encounter rates.
        environment->Update( );

        // Text output for the completion of each ten percent of the run 
        if( cumulativeTimeInSeconds >= ( unsigned )cumulativeTenthsOfRunTime ) {
            cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
            Logger::Get( )->LogMessage( "t = " + Convertor::Get( )->ToString( timeStep ) + Constants::cDataDelimiterValue + Constants::cWhiteSpaceCharacter + Convertor::Get( )->ToString( percentCount ) + "% completed." );
            percentCount += 10;
        }

        // Data collection
        if( timeStep % Parameters::Get( )->GetSamplingRate( ) == 0 ) {
            DataRecorder::Get( )->AddDataTo( "AxisTimeSteps", timeStep );
            isAlive = environment->RecordData( );
        }

        timeStep = timeStep + 1;
    } while( cumulativeTimeInSeconds < Parameters::Get( )->GetRunTimeInSeconds( ) && isAlive == true );

    if( cumulativeTimeInSeconds >= Parameters::Get( )->GetRunTimeInSeconds( ) ) {
        Logger::Get( )->LogMessage( "Main time loop complete." );
    } else {
        Logger::Get( )->LogMessage( "Heterotroph population crashed. Main time loop aborted." );
    }
    Logger::Get( )->LogMessage( "" );
    FileWriter fileWriter;
    fileWriter.WriteFiles( );
    Logger::Get( )->LogMessage( "Total run time " + Convertor::Get( )->ToString( timer.Stop( ) ) + "s" );

    delete environment;

    return 0;
}
