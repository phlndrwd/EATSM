#ifndef CONSTANTS
#define	CONSTANTS

#include <string>

#include "Types.h"

namespace Constants {

    enum eHeterotrophInitialisationEnum {
        eMultiple,
        eSingle
    };

    enum ePreferenceFunctionEnum {
        eParabolic,
        eNormal
    };

    enum eFunctionalResponseEnum {
        eType0, // Not used.
        eType1,
        eType2
    };

    enum eStarvationFunctionType {
        eLinear,
        eBetaExponential
    };

    enum eSwitchEnum {
        eOff,
        eOn
    };

    enum eGenomeIndices {
        eVolumeGene
    };

    enum eAxisVectors {
        eAxisAbstractTime,
        eAxisSizeClassMidPointValues,
        eAxisSizeClassBoundaryValues
    };

    const unsigned int cNumberOfAxisVectors = eAxisSizeClassBoundaryValues + 1;

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

    const unsigned int cNumberOfSizeClassDatums = eSizeClassAges + 1;

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

    const unsigned int cNumberOfTrophicDatums = eTrophicAges + 1;

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
        ePhytoplanktonVolume,
        eToPhytoplanktonFlux,
        eNutrientVolume,
        eToNutrientFlux
    };

    const unsigned int cNumberOfVectorDatums = eToNutrientFlux + 1;

    const std::string cVectorDatumNames[ cNumberOfVectorDatums ] = {
        "HeterotrophFrequency",
        "HeterotrophVolume",
        "HeterotrophApproximateVolume",
        "ToHeterotrophFlux",
        "InHeterotrophFlux",
        "PhytoplanktonVolume",
        "ToPhytoplanktonFlux",
        "NutrientVolume",
        "ToNutrientFlux"
    };

    enum eReturnCodes {
        eSuccessful,
        eIncorrectStatePath,
        eIncorrectParameterPath,
        eIncorrectVersion,
        eTooManyParameters,
        eNotEnoughParameters
    };

    enum eParameters {
        eRandomNumberSeed,
        eRunTimeInSeconds,
        eSamplingRate,
        eNumberOfSizeClasses,

        eHeterotrophInitialisationMethod,
        ePreferenceFunctionType,
        eFunctionalResponseType,
        eStarvationFunctionType,

        eInitialNutrientVolume,
        eInitialPhytoplanktonVolume,
        eInitialHeterotrophVolume,
        eMinimumHeterotrophVolume,

        eSmallestIndividualVolume,
        eLargestIndividualVolume,

        ePreferredPreyVolumeRatio,
        ePreferenceFunctionWidth,
        ePreferenceFunctionHeight,

        eSizeClassSubsetFraction,
        eHalfSaturationConstantFraction,

        eAssimilationEfficiency,
        eFractionalMetabolicExpensePerTimeStep,
        eMetabolicIndex,

        eMutationProbability,
        eMutationStandardDeviation
    };

    const unsigned int cNumberOfParameters = eMutationStandardDeviation + 1;

    const std::string cParameterHandles[ cNumberOfParameters ] = {
        "RandomNumberSeed",
        "t",
        "SamplingRate",
        "d",
        "HeterotrophInitialisationMethod",
        "PreferenceFunctionType",
        "FunctionalResponseType",
        "StarvationFunctionType",
        "Ninit",
        "Pinit",
        "Hinit",
        "Hmin",
        "vsmall",
        "vlarge",
        "beta",
        "c",
        "a",
        "alpha",
        "Kfrac",
        "gamma",
        "epsilon",
        "k",
        "Pmu",
        "sigma"
    };

    enum eDatumTypes {
        eIntegerVector,
        eIntegerMatrix,
        eFloatVector,
        eFloatMatrix
    };

    const unsigned int cMaximumNumberOfTrophicLevels = 11; // 0 = unclassified, 1 = primary, etc.
    const int cNaNValue = -10;

    const double cPi = 3.14159265358979323846264338327950288;

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
    const std::string cFolderDelimiter = "/";

    const std::string cParameterNameValueDelimiter = " = ";

    const char cParameterNameValueSpacer = '.';

    const std::string cAxisVectorNamesFileName = "MetaAxisVectorNames";
    const std::string cTrophicDatumNamesFileName = "MetaTrophicDatumNames";
    const std::string cVectorDatumNamesFileName = "MetaVectorDatumNames";
    const std::string cMatrixDatumNamesFileName = "MetaMatrixDatumNames";
    const std::string cVectorEnumIndicesFileName = "MetaVectorEnumIndicies";
    const std::string cMatrixEnumIndicesFileName = "MetaMatrixEnumIndicies";
    const std::string cConstantsFileName = "Constants";
    const std::string cSummaryFileName = "Summary";
    const std::string cStateFileName = "State";

    const std::string cOutputValueDelimiter = ", ";
    const std::string cWordDelimiter = " ";

    const std::string cExperimentCommandLineArgument = "-e";
    const std::string cRestartCommandLineArgument = "-r";
    const std::string cVersionCommandLineArgument = "-v";

    const char cStringSplitCharacter = '_';

    const unsigned int cReproductionFactor = 2;

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

