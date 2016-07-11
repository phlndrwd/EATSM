#ifndef CONSTANTS
#define	CONSTANTS

#include <string>

#include "Types.h"

namespace Constants {

    // Input data file enums
    enum eParametersMetadata {
        eParameterName,
        eParameterValue
    };

    enum eGenomeIndices {
        eVolumeGene
    };

    enum eAxisVectors {
        eAxisAbstractTime,
        eAxisSizeClassMidPointValues,
        eAxisSizeClassBoundaryValues
    };

    const unsigned cNumberOfAxisVectors = eAxisSizeClassBoundaryValues + 1;

    const std::string cAxisVectorNames[ cNumberOfAxisVectors ] = {
        "AxisAbstractTime",
        "AxisSizeClassMidPointValues",
        "AxisSizeClassBoundaryValues"
    };

    enum eSizeClassDatums {
        // Frequency datums
        eSizeClassSizes,
        eSizeClassVegetarianFrequencies,
        eSizeClassCarnivoreFrequencies,
        eSizeClassPreyFrequencies,
        eSizeClassStarvedFrequencies,
        eSizeClassParentFrequencies,
        eSizeClassChildFrequencies,
        eSizeClassVolumeMutantFrequencies,
        // Volume datums
        eSizeClassVolumes,
        eSizeClassApproximateVolumes,
        eSizeClassEffectivePreyVolumes,
        // Other
        eSizeClassGrowthRatios,
        eSizeClassCouplings,
        eSizeClassPreyVolumeRatios,
        eSizeClassFeedingProbabilities,
        eSizeClassTrophicClassifications,
        eSizeClassAges
    };

    const unsigned cNumberOfSizeClassDatums = eSizeClassAges + 1;

    const std::string cSizeClassDatumNames[ cNumberOfSizeClassDatums ] = {
        "SizeClassSizes",
        "SizeClassVegetarianFrequencies",
        "SizeClassCarnivoreFrequencies",
        "SizeClassPreyFrequencies",
        "SizeClassStarvedFrequencies",
        "SizeClassParentFrequencies",
        "SizeClassChildFrequencies",
        "SizeClassVolumeMutantFrequencies",

        "SizeClassVolumes",
        "SizeClassApproximateVolumes",
        "SizeClassEffectivePreyVolumes",

        "SizeClassGrowthRatios",
        "SizeClassCouplings",
        "SizeClassPreyVolumeRatios",
        "SizeClassFeedingProbabilities",
        "SizeClassTrophicClassifications",
        "SizeClassAges"
    };

    enum eTrophicDatums {
        eTrophicFrequencies,
        eTrophicVolumes,
        eTrophicAges
    };

    const unsigned cNumberOfTrophicDatums = eTrophicAges + 1;

    const std::string cTrophicDatumNames[ cNumberOfTrophicDatums ] = {
        "TrophicFrequencies",
        "TrophicVolumes",
        "TrophicAges"
    };

    enum eVectorDatums {
        eHeterotrophFrequency,
        eHeterotrophVolume,
        eHeterotrophApproximateVolume,
        eToHeterotrophFlux,
        eInHeterotrophFlux,
        eAutotrophVolume,
        eToPhytoplanktonFlux,
        eNutrientVolume,
        eToNutrientFlux
    };

    const unsigned cNumberOfVectorDatums = eToNutrientFlux + 1;

    const std::string cVectorDatumNames[ cNumberOfVectorDatums ] = {
        "HeterotrophFrequency",
        "HeterotrophVolume",
        "HeterotrophApproximateVolume",
        "ToHeterotrophFlux",
        "InHeterotrophFlux",
        "AutotrophVolume",
        "ToPhytoplanktonFlux",
        "NutrientVolume",
        "ToNutrientFlux"
    };

    enum eDatumTypes {
        eIntegerVector,
        eIntegerMatrix,
        eFloatVector,
        eFloatMatrix
    };
    
    
    const std::string cConfigurationDirectory = "./input/";
    const std::string cInputParametersFileName = "Parameters.csv";

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
    const std::string cOutputFileExtension = ".dat";
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

