#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <fstream>      // For std::i/ofstream
#include <sys/time.h>   // For struct timeval
#include <sys/stat.h>   // For mkdir
#include <map>          // For map

class Autotrophs;
class DataRecorder;
class Environment;
class Heterotrophs;
class HeterotrophData;
class HeterotrophProcessor;
class HeritableTraits;
class Individual;
class InitialState;
class MatrixDatum;
class Nutrient;
class Parameters;
class RandomSFMT;
class TimeStep;
class VectorDatum;

namespace Types {
    // Pointers
    typedef Autotrophs* AutotrophsPointer;
    typedef DataRecorder* DataRecorderPointer;
    typedef Environment* EnvironmentPointer;
    typedef Heterotrophs* HeterotrophsPointer;
    typedef HeterotrophData* HeterotrophDataPointer;
    typedef HeterotrophProcessor* HeterotrophProcessorPointer;
    typedef HeritableTraits* HeritableTraitsPointer;
    typedef Individual* IndividualPointer;
    typedef InitialState* InitialStatePointer;
    typedef MatrixDatum* MatrixDatumPointer;
    typedef Nutrient* NutrientPointer;
    typedef Parameters* ParametersPointer;
    typedef RandomSFMT* RandomSFMTPointer;
    typedef TimeStep* TimePointer;
    typedef VectorDatum* VectorDatumPointer;

    // Containers of class pointers
    typedef std::map< std::string, MatrixDatumPointer > MatrixDatumMap;
    typedef std::map< std::string, VectorDatumPointer > VectorDatumMap;

    typedef std::vector< IndividualPointer > IndividualVector;

    // Containers of containers of class pointers
    typedef std::vector< IndividualVector > IndividualMatrix;

    // Containers of primitives
    typedef std::map< std::string, double > DoubleMap;
    typedef std::map< std::string, double* > DoublePointerMap;
    typedef std::map< std::string, float > FloatMap;

    typedef std::vector< bool > BoolVector;
    typedef std::vector< double > DoubleVector;
    typedef std::vector< float > FloatVector;
    typedef std::vector< int > IntVector;
    typedef std::vector< std::string > StringVector;
    typedef std::vector< unsigned > UnsignedVector;

    // Containers of containers of primitives
    typedef std::map< std::string, FloatVector > FloatVectorMap;

    typedef std::vector< FloatVector > FloatMatrix;
    typedef std::vector< IntVector > IntMatrix;
    typedef std::vector< StringVector > StringMatrix;
}

#endif
