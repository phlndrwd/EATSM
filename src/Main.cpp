#include "Constants.h"
#include "Parameters.h"
#include "FileReader.h"
#include "Strings.h"
#include "Environment.h"
#include "DataRecorder.h"
#include "FileWriter.h"
#include "Timer.h"
#include "HeterotrophProcessor.h"
#include "TimeStep.h"

#include "RandomSFMT.h"

int main( int numberOfArguments, char* commandlineArguments[ ] ) {
    std::cout << Constants::cSystemName + " " + Constants::cSystemVersion + " Starting up..." << std::endl << std::endl;

    std::string parametersFile = "";
    std::string stateFile = "";

    bool commandLineArgumentsAreValid = true;

    switch( numberOfArguments ) {
        case 1:
            break;
        case 3:
        {
            std::string firstTerm = commandlineArguments[ 1 ];
            std::string secondTerm = commandlineArguments[ 2 ];

            if( firstTerm == Constants::cParameterFileCommand ) {
                parametersFile = secondTerm;
            } else if( firstTerm == Constants::cStateFileCommand ) {
                stateFile = secondTerm;
            } else {
                std::cout << "ERROR> Command \"" << firstTerm << "\" is not recognised. System exiting..." << std::endl;
                commandLineArgumentsAreValid = false;
            }
            break;
        }
        case 5:
        {
            std::string firstTerm = commandlineArguments[ 1 ];
            std::string secondTerm = commandlineArguments[ 2 ];
            std::string thirdTerm = commandlineArguments[ 3 ];
            std::string fourthTerm = commandlineArguments[ 4 ];

            if( firstTerm == Constants::cParameterFileCommand ) {
                if( secondTerm == Constants::cStateFileCommand ) {
                    parametersFile = thirdTerm;
                    stateFile = fourthTerm;
                } else if( thirdTerm == Constants::cStateFileCommand ) {
                    parametersFile = secondTerm;
                    stateFile = fourthTerm;
                } else {
                    std::cout << "ERROR> Either \"" << secondTerm << "\" or \"" << thirdTerm << "\" is an unrecognised command. System exiting..." << std::endl;
                    commandLineArgumentsAreValid = false;
                }
            } else if( firstTerm == Constants::cStateFileCommand ) {
                if( secondTerm == Constants::cParameterFileCommand ) {
                    stateFile = thirdTerm;
                    parametersFile = fourthTerm;
                } else if( thirdTerm == Constants::cParameterFileCommand ) {
                    stateFile = secondTerm;
                    parametersFile = fourthTerm;
                } else {
                    std::cout << "ERROR> Either \"" << secondTerm << "\" or \"" << thirdTerm << "\" is an unrecognised command. System exiting..." << std::endl;
                    commandLineArgumentsAreValid = false;
                }
            } else {
                std::cout << "ERROR> Command \"" << firstTerm << "\" is not recognised. System exiting..." << std::endl;
                commandLineArgumentsAreValid = false;
            }
            break;
        }
        default:
        {
            std::cout << "ERROR> Incorrect number of commands. System exiting..." << std::endl;
            commandLineArgumentsAreValid = false;
            break;
        }
    }
    
    if( commandLineArgumentsAreValid ) {
        FileReader fileReader;
        fileReader.ReadInputFiles( parametersFile, stateFile );
        Timer timer = Timer( true );
        FileWriter fileWriter; // Created here to initialise output directory
        Environment environment;

        double oneTenthOfRunTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( ) / 10.0;
        double cumulativeTenthsOfRunTime = 0;
        unsigned percentCount = 0;
        bool isAlive = true;

        std::cout << "Starting main time loop..." << std::endl;
        do {
            // Update before data collection; calculates essential variables for encounter rates.
            environment.Update( );

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
                isAlive = environment.RecordData( );
            }
            TimeStep::Get( )->IncrementTimeStep( );
        } while( timer.Elapsed( ) < Parameters::Get( )->GetRunTimeInSeconds( ) && isAlive == true );
        if( timer.Elapsed( ) >= Parameters::Get( )->GetRunTimeInSeconds( ) ) {
            std::cout << "Main time loop complete." << std::endl << std::endl;
        } else {
            std::cout << "Heterotroph population crashed. Main time loop aborted." << std::endl << std::endl;
        }

        fileWriter.WriteOutputData( &environment );
        std::cout << "Total run time " << timer.Stop( ) << "s" << std::endl;
    }
    return 0;
}
