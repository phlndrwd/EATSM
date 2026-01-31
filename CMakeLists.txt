cmake_minimum_required(VERSION 3.25)

project(eatsm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY ${CMAKE_SOURCE_DIR}/input
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(eatsm
    src/Data/HeterotrophData.cpp
    src/Data/VectorDatum.cpp
    src/Data/MatrixDatum.cpp
    src/Input/FileReader.cpp
    src/Input/InitialState.cpp
    src/Input/Parameters.cpp
    src/Model/Autotrophs.cpp       
    src/Model/HeterotrophProcessor.cpp
    src/Model/Environment.cpp      
    src/Model/Heterotrophs.cpp
    src/Model/HeritableTraits.cpp  
    src/Model/Nutrient.cpp
    src/Model/Heterotroph.cpp      
    src/Model/TimeStep.cpp
    src/Output/DataRecorder.cpp
    src/Output/FileWriter.cpp
    src/Tools/Date.cpp
    src/Tools/Strings.cpp
    src/Tools/RandomSimple.cpp
    src/Tools/Timer.cpp
    src/Main.cpp
)

target_include_directories(eatsm
    PRIVATE
    	src/Data
        src/Input
	src/Model
	src/Output
	src/Tools
)

