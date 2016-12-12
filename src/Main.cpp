#include "Constants.h"
#include "Parameters.h"
#include "FileReader.h"
#include "StringTools.h"
#include "Environment.h"
#include "DataRecorder.h"
#include "FileWriter.h"
#include "Timer.h"
#include "HeterotrophProcessor.h"
#include "TimeStep.h"

#include "RandomSFMT.h"

int main( int numberOfArguments, char* commandlineArguments[ ] ) {
    std:: cout << Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up..." << std::endl << std::endl;

    FileReader fileReader;

    std::string stateFile = "";
    if( numberOfArguments > 1 ) stateFile = commandlineArguments[ 1 ];

    if( fileReader.ReadInputFiles( stateFile ) == true ) {
        std::cout << std::endl;
        
        Timer timer = Timer( true );
        FileWriter fileWriter;
        Types::EnvironmentPointer environment = new Environment( );
        std::cout << std::endl;

        double oneTenthOfRunTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( ) / 10.0;
        double cumulativeTenthsOfRunTime = 0;
        unsigned percentCount = 0;
        bool isAlive = true;

        std::cout << "Starting main time loop..." << std::endl;
        do {
            // Update before data collection; calculates essential variables for encounter rates.
            environment->Update( );

            // Text output at the completion of each ten percent of the run 
            if( timer.Elapsed( ) >= ( unsigned )cumulativeTenthsOfRunTime ) {
                cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
                std::cout << "t = " << TimeStep::Get( )->GetTimeStep( ) << Constants::cDataDelimiterValue << Constants::cWhiteSpaceCharacter << percentCount << "% completed." << std::endl;
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
            std::cout << "Main time loop complete." << std::endl << std::endl;
        } else {
            std::cout << "Heterotroph population crashed. Main time loop aborted." << std::endl << std::endl;
        }

        fileWriter.WriteOutputData( environment );
        std::cout << "Total run time " << timer.Stop( ) << "s" << std::endl;

        delete environment;
    } else {
        std::cout << "ERROR> File reading failed. System exiting..." << std::endl;
    }

    return 0;
}
