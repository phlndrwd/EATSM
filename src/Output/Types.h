#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <fstream>      // For std::i/ofstream
#include <sys/time.h>   // For struct timeval
#include <sys/stat.h>   // For mkdir
#include <map>          // For map

class Autotroph;
class Convertor;
class DataRecorder;
class Environment;
class Genome;
class Heterotrophs;
class HeterotrophData;
class HeterotrophProcessor;
class Individual;
class Logger;
class Maths;
class MatrixDatum;
class Nutrient;
class Parameters;
class RandomDefault;
class RandomInterface;
class VectorDatum;

namespace Types {
    // Pointers
    typedef Autotroph* AutotrophPointer;
    typedef Convertor* ConvertorPointer;
    typedef DataRecorder* DataRecorderPointer;
    typedef Environment* EnvironmentPointer;
    typedef Genome* GenomePointer;
    typedef Heterotrophs* HeterotrophsPointer;
    typedef HeterotrophData* HeterotrophDataPointer;
    typedef HeterotrophProcessor* HeterotrophProcessorPointer;
    typedef Individual* IndividualPointer;
    typedef Logger* LoggerPointer;
    typedef Maths* MathsPointer;
    typedef MatrixDatum* MatrixDatumPointer;
    typedef Nutrient* NutrientPointer;
    typedef Parameters* ParametersPointer;
    typedef RandomDefault* RandomDefaultPointer;
    typedef RandomInterface* RandomInterfacePointer;
    typedef VectorDatum* VectorDatumPointer;

    // Containers of pointers
    typedef std::map< std::string, MatrixDatumPointer > MatrixDatumMap;
    typedef std::map< std::string, VectorDatumPointer > VectorDatumMap;

    typedef std::vector< IndividualPointer > IndividualArray;
    
    // Containers of containers of pointers
    typedef std::vector< IndividualArray > IndividualMatrix;

    // Containers of primitives
    typedef std::vector< bool > BoolVector;
    typedef std::vector< double > DoubleVector;
    typedef std::vector< float > FloatVector;
    typedef std::vector< int > IntVector;
    typedef std::vector< std::string > StringVector;
    typedef std::vector< unsigned > UnsignedVector;

    // Containers of containers of primitives
    typedef std::vector< FloatVector > FloatMatrix;
    typedef std::vector< IntVector > IntMatrix;
    typedef std::vector< StringVector > StringMatrix;
}

#endif
