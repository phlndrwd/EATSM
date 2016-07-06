#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Input/FileReader.o \
	${OBJECTDIR}/src/Input/Parameters.o \
	${OBJECTDIR}/src/Input/RestartState.o \
	${OBJECTDIR}/src/Main.o \
	${OBJECTDIR}/src/Model/Environment.o \
	${OBJECTDIR}/src/Model/Genome.o \
	${OBJECTDIR}/src/Model/HeterotrophProcessor.o \
	${OBJECTDIR}/src/Model/Heterotrophs.o \
	${OBJECTDIR}/src/Model/Individual.o \
	${OBJECTDIR}/src/Model/Nutrient.o \
	${OBJECTDIR}/src/Model/Phytoplankton.o \
	${OBJECTDIR}/src/Output/DataRecorder.o \
	${OBJECTDIR}/src/Output/Datum.o \
	${OBJECTDIR}/src/Output/FileWriter.o \
	${OBJECTDIR}/src/Output/FloatMatrixDatum.o \
	${OBJECTDIR}/src/Output/FloatVectorDatum.o \
	${OBJECTDIR}/src/Output/HeterotrophData.o \
	${OBJECTDIR}/src/Output/IntegerMatrixDatum.o \
	${OBJECTDIR}/src/Output/IntegerVectorDatum.o \
	${OBJECTDIR}/src/Output/Logger.o \
	${OBJECTDIR}/src/Tools/Convertor.o \
	${OBJECTDIR}/src/Tools/DateTime.o \
	${OBJECTDIR}/src/Tools/Maths.o \
	${OBJECTDIR}/src/Tools/Random.o \
	${OBJECTDIR}/src/Tools/RandomSFMT.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eatsm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eatsm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eatsm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/Input/FileReader.o: src/Input/FileReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/FileReader.o src/Input/FileReader.cpp

${OBJECTDIR}/src/Input/Parameters.o: src/Input/Parameters.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/Parameters.o src/Input/Parameters.cpp

${OBJECTDIR}/src/Input/RestartState.o: src/Input/RestartState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/RestartState.o src/Input/RestartState.cpp

${OBJECTDIR}/src/Main.o: src/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Main.o src/Main.cpp

${OBJECTDIR}/src/Model/Environment.o: src/Model/Environment.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Environment.o src/Model/Environment.cpp

${OBJECTDIR}/src/Model/Genome.o: src/Model/Genome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Genome.o src/Model/Genome.cpp

${OBJECTDIR}/src/Model/HeterotrophProcessor.o: src/Model/HeterotrophProcessor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/HeterotrophProcessor.o src/Model/HeterotrophProcessor.cpp

${OBJECTDIR}/src/Model/Heterotrophs.o: src/Model/Heterotrophs.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Heterotrophs.o src/Model/Heterotrophs.cpp

${OBJECTDIR}/src/Model/Individual.o: src/Model/Individual.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Individual.o src/Model/Individual.cpp

${OBJECTDIR}/src/Model/Nutrient.o: src/Model/Nutrient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Nutrient.o src/Model/Nutrient.cpp

${OBJECTDIR}/src/Model/Phytoplankton.o: src/Model/Phytoplankton.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Phytoplankton.o src/Model/Phytoplankton.cpp

${OBJECTDIR}/src/Output/DataRecorder.o: src/Output/DataRecorder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/DataRecorder.o src/Output/DataRecorder.cpp

${OBJECTDIR}/src/Output/Datum.o: src/Output/Datum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/Datum.o src/Output/Datum.cpp

${OBJECTDIR}/src/Output/FileWriter.o: src/Output/FileWriter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/FileWriter.o src/Output/FileWriter.cpp

${OBJECTDIR}/src/Output/FloatMatrixDatum.o: src/Output/FloatMatrixDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/FloatMatrixDatum.o src/Output/FloatMatrixDatum.cpp

${OBJECTDIR}/src/Output/FloatVectorDatum.o: src/Output/FloatVectorDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/FloatVectorDatum.o src/Output/FloatVectorDatum.cpp

${OBJECTDIR}/src/Output/HeterotrophData.o: src/Output/HeterotrophData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/HeterotrophData.o src/Output/HeterotrophData.cpp

${OBJECTDIR}/src/Output/IntegerMatrixDatum.o: src/Output/IntegerMatrixDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/IntegerMatrixDatum.o src/Output/IntegerMatrixDatum.cpp

${OBJECTDIR}/src/Output/IntegerVectorDatum.o: src/Output/IntegerVectorDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/IntegerVectorDatum.o src/Output/IntegerVectorDatum.cpp

${OBJECTDIR}/src/Output/Logger.o: src/Output/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/Logger.o src/Output/Logger.cpp

${OBJECTDIR}/src/Tools/Convertor.o: src/Tools/Convertor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Convertor.o src/Tools/Convertor.cpp

${OBJECTDIR}/src/Tools/DateTime.o: src/Tools/DateTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/DateTime.o src/Tools/DateTime.cpp

${OBJECTDIR}/src/Tools/Maths.o: src/Tools/Maths.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Maths.o src/Tools/Maths.cpp

${OBJECTDIR}/src/Tools/Random.o: src/Tools/Random.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Random.o src/Tools/Random.cpp

${OBJECTDIR}/src/Tools/RandomSFMT.o: src/Tools/RandomSFMT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -Isrc/Input/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/RandomSFMT.o src/Tools/RandomSFMT.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eatsm

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
