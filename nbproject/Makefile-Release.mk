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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Data/ConsumptionEvent.o \
	${OBJECTDIR}/src/Data/DataTag.o \
	${OBJECTDIR}/src/Data/HeterotrophData.o \
	${OBJECTDIR}/src/Data/MatrixDatum.o \
	${OBJECTDIR}/src/Data/Tagger.o \
	${OBJECTDIR}/src/Data/VectorDatum.o \
	${OBJECTDIR}/src/Input/FileReader.o \
	${OBJECTDIR}/src/Input/InitialState.o \
	${OBJECTDIR}/src/Input/Parameters.o \
	${OBJECTDIR}/src/Main.o \
	${OBJECTDIR}/src/Model/Autotrophs.o \
	${OBJECTDIR}/src/Model/Environment.o \
	${OBJECTDIR}/src/Model/Genome.o \
	${OBJECTDIR}/src/Model/HeterotrophProcessor.o \
	${OBJECTDIR}/src/Model/Heterotrophs.o \
	${OBJECTDIR}/src/Model/Individual.o \
	${OBJECTDIR}/src/Model/Nutrient.o \
	${OBJECTDIR}/src/Model/TimeStep.o \
	${OBJECTDIR}/src/Output/DataRecorder.o \
	${OBJECTDIR}/src/Output/FileWriter.o \
	${OBJECTDIR}/src/Tools/Date.o \
	${OBJECTDIR}/src/Tools/RandomSFMT.o \
	${OBJECTDIR}/src/Tools/Strings.o \
	${OBJECTDIR}/src/Tools/Timer.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-O1 -std=c++11 -march=core2 -mfpmath=sse
CXXFLAGS=-O1 -std=c++11 -march=core2 -mfpmath=sse

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

${OBJECTDIR}/src/Data/ConsumptionEvent.o: src/Data/ConsumptionEvent.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/ConsumptionEvent.o src/Data/ConsumptionEvent.cpp

${OBJECTDIR}/src/Data/DataTag.o: src/Data/DataTag.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/DataTag.o src/Data/DataTag.cpp

${OBJECTDIR}/src/Data/HeterotrophData.o: src/Data/HeterotrophData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/HeterotrophData.o src/Data/HeterotrophData.cpp

${OBJECTDIR}/src/Data/MatrixDatum.o: src/Data/MatrixDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/MatrixDatum.o src/Data/MatrixDatum.cpp

${OBJECTDIR}/src/Data/Tagger.o: src/Data/Tagger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/Tagger.o src/Data/Tagger.cpp

${OBJECTDIR}/src/Data/VectorDatum.o: src/Data/VectorDatum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Data
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data/VectorDatum.o src/Data/VectorDatum.cpp

${OBJECTDIR}/src/Input/FileReader.o: src/Input/FileReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/FileReader.o src/Input/FileReader.cpp

${OBJECTDIR}/src/Input/InitialState.o: src/Input/InitialState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/InitialState.o src/Input/InitialState.cpp

${OBJECTDIR}/src/Input/Parameters.o: src/Input/Parameters.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Input
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Input/Parameters.o src/Input/Parameters.cpp

${OBJECTDIR}/src/Main.o: src/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Main.o src/Main.cpp

${OBJECTDIR}/src/Model/Autotrophs.o: src/Model/Autotrophs.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Autotrophs.o src/Model/Autotrophs.cpp

${OBJECTDIR}/src/Model/Environment.o: src/Model/Environment.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Environment.o src/Model/Environment.cpp

${OBJECTDIR}/src/Model/Genome.o: src/Model/Genome.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Genome.o src/Model/Genome.cpp

${OBJECTDIR}/src/Model/HeterotrophProcessor.o: src/Model/HeterotrophProcessor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/HeterotrophProcessor.o src/Model/HeterotrophProcessor.cpp

${OBJECTDIR}/src/Model/Heterotrophs.o: src/Model/Heterotrophs.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Heterotrophs.o src/Model/Heterotrophs.cpp

${OBJECTDIR}/src/Model/Individual.o: src/Model/Individual.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Individual.o src/Model/Individual.cpp

${OBJECTDIR}/src/Model/Nutrient.o: src/Model/Nutrient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/Nutrient.o src/Model/Nutrient.cpp

${OBJECTDIR}/src/Model/TimeStep.o: src/Model/TimeStep.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Model/TimeStep.o src/Model/TimeStep.cpp

${OBJECTDIR}/src/Output/DataRecorder.o: src/Output/DataRecorder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/DataRecorder.o src/Output/DataRecorder.cpp

${OBJECTDIR}/src/Output/FileWriter.o: src/Output/FileWriter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Output
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Output/FileWriter.o src/Output/FileWriter.cpp

${OBJECTDIR}/src/Tools/Date.o: src/Tools/Date.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Date.o src/Tools/Date.cpp

${OBJECTDIR}/src/Tools/RandomSFMT.o: src/Tools/RandomSFMT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/RandomSFMT.o src/Tools/RandomSFMT.cpp

${OBJECTDIR}/src/Tools/Strings.o: src/Tools/Strings.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Strings.o src/Tools/Strings.cpp

${OBJECTDIR}/src/Tools/Timer.o: src/Tools/Timer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Tools
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Isrc/Input/ -Isrc/Data/ -Isrc/Model/ -Isrc/Output/ -Isrc/Tools/ -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Tools/Timer.o src/Tools/Timer.cpp

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
