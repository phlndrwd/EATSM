#ifndef CONSTANTS
#define	CONSTANTS

#include <string>

#include "Types.h"

namespace Constants {

    // Input data file enums
    enum eInputParametersMetadata {
        eParameterName,
        eParameterValue
    };
    
    enum eOutputControlParameters {
        eDatumName,
        eDatumType
    };

    enum eGenomeIndices {
        eVolumeGene
    };

    
    const std::string cConfigurationDirectory = "./input/";
    const std::string cInputParametersFileName = "Parameters.csv";
    const std::string cOutputParametersFileName = "OutputControlParameters.csv";
    const std::string cHeterotrophInitialisationFileName = "HeterotrophInitialisation.csv";
    
    const std::string cVectorDatumTypeName = "vector";
    const std::string cMatrixDatumTypeName = "matrix";
    
    const unsigned cMaximumNumberOfTrophicLevels = 11; // 0 = unclassified, 1 = primary, etc.
    const int cMissingValue = -9999;

    const int cDateTimeBufferSize = 25;
    const int cOutputFolderPermissions = 0777;
    const int cMinimumParameterNameValueSpacing = 3;


    const double cReproductionScalingFactor = 2;
    const double cMinimumFractionalVolume = 0.5;
    const int cNumberOfIndividualRestartParameters = 5;

    const std::string cCompleteDateFormat = "%c";
    const std::string cDataSetNameFormat = "%Y-%m-%d_%H-%M-%S";
    const std::string cOutputDirectoryName = "output";
    const std::string cOutputFileExtension = ".csv";
    const std::string cTextFileExtension = ".txt";

    const std::string cParameterNameValueDelimiter = " = ";

    const char cParameterNameValueSpacer = '.';

    const std::string cAxisVectorNamesFileName = "MetaAxisVectorNames";
    const std::string cTrophicDatumNamesFileName = "MetaTrophicDatumNames";
    const std::string cVectorDatumNamesFileName = "MetaVectorDatumNames";
    const std::string cMatrixDatumNamesFileName = "MetaMatrixDatumNames";
    const std::string cVectorEnumIndicesFileName = "MetaVectorEnumIndicies";
    const std::string cMatrixEnumIndicesFileName = "MetaMatrixEnumIndicies";

    const std::string cWordDelimiter = " ";
    
    const char cFolderDelimiter = '/';
    const char cStringSplitCharacter = '_';
    const char cWhiteSpaceCharacter = ' ';
    const char cDataDelimiterValue = ',';
    const char cTextFileCommentCharacter = '#';

    const unsigned cReproductionFactor = 2;

    // Pseudo-Random Number Generator constants
    const int cUniformIntMin = -2147483647;
    const int cUniformIntMax = 2147483647;

    const float cUniformDoubleMin = 0.0;
    const float cUniformDoubleMax = 1.0;

    const float cNormalMean = 0.0;
    const float cNormalStdDev = 1.0;

    const std::string cSystemName = "EATSM";
    const std::string cSystemVersion = "0.1";
    const std::string cSystemDate = "06/07/2016";
    const std::string cSystemTime = "16:02";
}

#endif

