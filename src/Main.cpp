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
#include "Date.h"

int main( int numberOfArguments, char* commandlineArguments[ ] ) {
    std::string parametersFile = "";
    std::string stateFile = "";

    bool commandLineArgumentsAreValid = true;

    switch( numberOfArguments ) {
        case 1:
            break;

        case 2:
        {
            commandLineArgumentsAreValid = false;
            std::string command = commandlineArguments[ 1 ];
            if( command == Constants::cVersionCommand ) {
                std::cout << Constants::cSystemName << std::endl;
                std::cout << Constants::cSystemVersion << std::endl;
                std::cout << Constants::cSystemDate << std::endl;
                std::cout << Constants::cSystemTime << std::endl;
            } else
                std::cout << "ERROR> Command \"" << command << "\" is not recognised. System exiting..." << std::endl;
            
            break;
        }
        case 3:
        {
            std::string command = commandlineArguments[ 1 ];
            std::string filePath = commandlineArguments[ 2 ];

            if( command == Constants::cParameterFileCommand ) {
                parametersFile = filePath;
            } else if( command == Constants::cStateFileCommand ) {
                stateFile = filePath;
            } else {
                std::cout << "ERROR> Command \"" << command << "\" is not recognised. System exiting..." << std::endl;
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
        std::cout << Constants::cSystemName + " " + Constants::cSystemVersion + " starting on " << Date::GetDateAndTimeString( ) << "..." << std::endl;
        std::cout << "Due to complete on " << Date::GetDateAndTimeString( Constants::cCompleteDateFormat, Parameters::Get()->GetRunTimeInSeconds( ) ) << std::endl << std::endl;
        FileReader fileReader;
        fileReader.ReadInputFiles( parametersFile, stateFile );
        Timer timer = Timer( true );
        FileWriter fileWriter; // Created here to initialise output directory
        Environment environment;

        double oneTenthOfRunTimeInSeconds = Parameters::Get( )->GetRunTimeInSeconds( ) / 10.0;
        double cumulativeTenthsOfRunTime = 0;
        bool isAlive = true;

        std::cout << "Starting main time loop..." << std::endl;
        do {
            // Update before data collection; calculates essential variables for encounter rates.
            environment.Update( );

            // Text output at the completion of each ten percent of the run 
            if( timer.Elapsed( ) >= ( unsigned )cumulativeTenthsOfRunTime ) {
                cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
                std::cout << "t = " << TimeStep::Get( )->GetTimeStep( ) << Constants::cDataDelimiterValue << Constants::cWhiteSpaceCharacter << timer.RemainingString( ) << " remaining at " << Date::GetDateAndTimeString( ) << "..." << std::endl;
            }

            // Data collection
            if( TimeStep::Get( )->DoRecordData( ) == true ) {
                DataRecorder::Get( )->AddDataTo( "AxisTimeSteps", TimeStep::Get( )->GetTimeStep( ) );
                DataRecorder::Get( )->AddDataTo( "TimeSampling", timer.Split( ) );
                isAlive = environment.RecordData( );
            }
            TimeStep::Get( )->IncrementTimeStep( );
            
            //std::cout << "Elapsed> " << timer.ElapsedString() << std::endl;
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
