#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <fstream>      // For std::i/ofstream
#include <sys/time.h>   // For struct timeval
#include <sys/stat.h>   // For mkdir
#include <map>          // For map

class Autotrophs;
class ConsumptionEvent;
class Convertor;
class DataRecorder;
class DataTag;
class Environment;
class Genome;
class Heterotrophs;
class HeterotrophData;
class HeterotrophProcessor;
class Individual;
class InitialState;
class Logger;
class Maths;
class MatrixDatum;
class Nutrient;
class Parameters;
class RandomSFMT;
class Tagger;
class Time;
class VectorDatum;

namespace Types {
    // Pointers
    typedef Autotrophs* AutotrophsPointer;
    typedef ConsumptionEvent* ConsumptionEventPointer;
    typedef Convertor* ConvertorPointer;
    typedef DataRecorder* DataRecorderPointer;
    typedef DataTag* DataTagPointer;
    typedef Environment* EnvironmentPointer;
    typedef Genome* GenomePointer;
    typedef Heterotrophs* HeterotrophsPointer;
    typedef HeterotrophData* HeterotrophDataPointer;
    typedef HeterotrophProcessor* HeterotrophProcessorPointer;
    typedef Individual* IndividualPointer;
    typedef InitialState* InitialStatePointer;
    typedef Logger* LoggerPointer;
    typedef Maths* MathsPointer;
    typedef MatrixDatum* MatrixDatumPointer;
    typedef Nutrient* NutrientPointer;
    typedef Parameters* ParametersPointer;
    typedef RandomSFMT* RandomSFMTPointer;
    typedef Tagger* TaggerPointer;
    typedef Time* TimePointer;
    typedef VectorDatum* VectorDatumPointer;

    // Containers of class pointers
    typedef std::map< std::string, MatrixDatumPointer > MatrixDatumMap;
    typedef std::map< std::string, VectorDatumPointer > VectorDatumMap;

    typedef std::vector< ConsumptionEventPointer > ConsumptionEventVector;
    typedef std::vector< DataTagPointer > DataTagVector;
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
