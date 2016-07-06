#include "FileReader.h"

#include "RestartState.h"
#include "Parameters.h"
#include "Individual.h"
#include "Logger.h"
#include "Genome.h"
#include "Convertor.h"

FileReader::FileReader( ) {

    mParameterIndex = 0;
}

FileReader::~FileReader( ) {
    mParametersFilePath.clear( );
}

Constants::eReturnCodes FileReader::ReadParametersFile( const std::string parametersFilePath ) {

    Constants::eReturnCodes returnCode = Constants::eSuccessful;

    mParametersFilePath = parametersFilePath;

    Types::InputFileStream parametersFileStream;
    parametersFileStream.open( mParametersFilePath.c_str( ) );

    bool isVersionSet = false;
    bool isExperimentSet = false;
    bool experimentIsNext = false;

    std::string readWord;

    if( parametersFileStream.is_open( ) ) {
        while( !parametersFileStream.eof( ) ) {

            parametersFileStream >> readWord;

            double readValue = 0;

            if( readWord == Constants::cSystemVersion && isVersionSet == false ) {
                isVersionSet = true;
            } else if( readWord == "=" && experimentIsNext == false && isExperimentSet == false ) {
                experimentIsNext = true;
            } else if( experimentIsNext == true && isExperimentSet == false ) {
                Parameters::Get( )->SetExperimentName( readWord );
                isExperimentSet = true;
            } else {
                readValue = Convertor::Get( )->StringToNumber( readWord );
            }

            if( readValue != 0 || readWord == "0" ) {
                SetParameter( readValue );
            }
        }

        parametersFileStream.close( );

        if( isVersionSet == false ) {
            returnCode = Constants::eIncorrectVersion;
        }

    } else {
        returnCode = Constants::eIncorrectParameterPath;
    }

    return returnCode;
}

 Constants::eReturnCodes FileReader::Restart( const std::string restartPath ) {

    Constants::eReturnCodes returnCode = Constants::eSuccessful;

    mRestartPath = restartPath;

    if( mRestartPath.substr( mRestartPath.size( ) - 1, 1 ) != Constants::cFolderDelimiter ) {
        mRestartPath.append( Constants::cFolderDelimiter );
    }

    mStateFilePath = mRestartPath;
    mParametersFilePath = mRestartPath;

    mParametersFilePath.append( Constants::cConstantsFileName );
    mParametersFilePath.append( Constants::cTextFileExtension );

    returnCode = ReadParametersFile( mParametersFilePath  );

    if( returnCode == Constants::eSuccessful ) {
        Parameters::Get( )->CalculateSizeClassVectors( );
        Parameters::Get( )->CalculateVariables( );
        returnCode = ReadStateFile( );
    }

    if( returnCode == Constants::eSuccessful ) {
        RestartState::Get( )->SetIsRestart( true );
    } else {
        RestartState::Get( )->SetIsRestart( false );
    }

    return returnCode;
}

 Constants::eReturnCodes FileReader::ReadStateFile( ) {

    Constants::eReturnCodes returnCode = Constants::eSuccessful;

    Types::InputFileStream stateFileStream;

    mStateFilePath.append( Constants::cStateFileName );
    mStateFilePath.append( Constants::cTextFileExtension );

    stateFileStream.open( mStateFilePath.c_str( ) );

    if( stateFileStream.is_open( ) ) {

        unsigned int stateFileParameterNumber = 0;
        unsigned int individualIndex = 0;

        std::string readWord;

        Types::DoubleVector genomeValues;
        unsigned int sizeClassIndex = 0;
        double actualVolume = 0;
        double trophicLevel = 0;
        unsigned int age = 0;

        while( !stateFileStream.eof( ) ) {

            stateFileStream >> readWord;

            double readValue = Convertor::Get( )->StringToNumber( readWord );

            if( readValue != 0 || readWord == "0" ) {
                switch( stateFileParameterNumber ) {
                    case 0:
                        RestartState::Get( )->SetStartingTimeStep( ( unsigned int )readValue );
                        break;

                    case 1:
                        RestartState::Get( )->SetNutrientVolume( readValue );
                        break;

                    case 2:
                        RestartState::Get( )->SetPhytoplanktonVolume( readValue );
                        break;

                    default:
                    {
                        Types::IndividualPointer individual;

                        unsigned int individualParameterIndex = ( stateFileParameterNumber - 3 ) % Constants::cNumberOfIndividualRestartParameters;

                        switch( individualParameterIndex ) {

                            case 0:
                                genomeValues.push_back( readValue );
                                break;

                            case 1:
                                actualVolume = readValue;
                                break;

                            case 2:
                                trophicLevel = readValue;
                                break;

                            case 3:
                                sizeClassIndex = ( unsigned int )readValue;
                                break;

                            case 4:
                            {
                                age = ( unsigned int )readValue;

                                Types::GenomePointer genome = new Genome( genomeValues );
                                individual = new Individual( genome, actualVolume, trophicLevel, sizeClassIndex, age );

                                RestartState::Get( )->AddIndividual( individual );
                                individualIndex = individualIndex + 1;

                                genomeValues.clear( );
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            stateFileParameterNumber = stateFileParameterNumber + 1;
        }
        stateFileStream.close( );

    } else {
        returnCode = Constants::eIncorrectStatePath;
    }

    return returnCode;
}

void FileReader::SetParameter( const double value ) {

    switch( mParameterIndex ) {

        case Constants::eRandomNumberSeed:
            Parameters::Get( )->SetRandomNumberSeed( ( unsigned int )value );
            break;

        case Constants::eRunTimeInSeconds:
            Parameters::Get( )->SetRunTimeInSeconds( ( unsigned int )value );
            break;

        case Constants::eSamplingRate:
            Parameters::Get( )->SetSamplingRate( ( unsigned int )value );
            break;

        case Constants::eNumberOfSizeClasses:
            Parameters::Get( )->SetNumberOfSizeClasses( ( unsigned int )value );
            break;

        case Constants::eHeterotrophInitialisationMethod:
            Parameters::Get( )->SetHeterotrophInitialisationMethod( ( unsigned int )value );
            break;

        case Constants::ePreferenceFunctionType:
            Parameters::Get( )->SetPreferenceFunctionType( ( unsigned int )value );
            break;

        case Constants::eFunctionalResponseType:
            Parameters::Get( )->SetFunctionalResponseType( ( unsigned int )value );
            break;

        case Constants::eInitialNutrientVolume:
            Parameters::Get( )->SetInitialNutrientVolume( value );
            break;

        case Constants::eInitialPhytoplanktonVolume:
            Parameters::Get( )->SetInitialPhytoplanktonVolume( value );
            break;

        case Constants::eInitialHeterotrophVolume:
            Parameters::Get( )->SetInitialHeterotrophVolume( value );
            break;

        case Constants::eMinimumHeterotrophVolume:
            Parameters::Get( )->SetMinimumHeterotrophVolume( value );
            break;

        case Constants::eSmallestIndividualVolume:
            Parameters::Get( )->SetSmallestIndividualVolume( value );
            break;

        case Constants::eLargestIndividualVolume:
            Parameters::Get( )->SetLargestIndividualVolume( value );
            break;

        case Constants::ePreferredPreyVolumeRatio:
            Parameters::Get( )->SetPreferredPreyVolumeRatio( ( unsigned int )value );
            break;

        case Constants::ePreferenceFunctionWidth:
            Parameters::Get( )->SetPreferenceFunctionWidth( value );
            break;

        case Constants::ePreferenceFunctionHeight:
            Parameters::Get( )->SetPreferenceFunctionHeight( value );
            break;

        case Constants::eSizeClassSubsetFraction:
            Parameters::Get( )->SetSizeClassSubsetFraction( value );
            break;

        case Constants::eHalfSaturationConstantFraction:
            Parameters::Get( )->SetHalfSaturationConstantFraction( value );
            break;

        case Constants::eAssimilationEfficiency:
            Parameters::Get( )->SetAssimilationEfficiency( value );
            break;

        case Constants::eFractionalMetabolicExpensePerTimeStep:
            Parameters::Get( )->SetFractionalMetabolicExpensePerTimeStep( value );
            break;

        case Constants::eMetabolicIndex:
            Parameters::Get( )->SetMetabolicIndex( value );
            break;

        case Constants::eMutationProbability:
            Parameters::Get( )->SetMutationProbability( value );
            break;

        case Constants::eMutationStandardDeviation:
            Parameters::Get( )->SetMutationStandardDeviation( value );
            break;

        default:
            // Do nothing
            break;
    }

    mParameterIndex = mParameterIndex + 1;
}

const std::string FileReader::GetParametersFilePath( ) const {
    return mParametersFilePath;
}
