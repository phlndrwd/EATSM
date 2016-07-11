#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <fstream>      // For std::i/ofstream
#include <sys/time.h>   // For struct timeval
#include <sys/stat.h>   // For mkdir
#include <map>          // For map

class Environment;
class Nutrient;
class Autotroph;
class Heterotrophs;
class Individual;
class Genome;

class HeterotrophData;
class HeterotrophProcessor;

class Parameters;
class RandomDefault;
class RandomInterface;
class Logger;
class Convertor;
class Maths;
class FileReader;
class FileWriter;
class DataRecorder;
class Datum;
class IntegerVectorDatum;
class FloatVectorDatum;
class IntegerMatrixDatum;
class FloatMatrixDatum;

namespace Types {

    typedef Environment* EnvironmentPointer;
    typedef Nutrient* NutrientPointer;
    typedef Autotroph* AutotrophPointer;
    typedef Heterotrophs* HeterotrophsPointer;
    typedef Individual* IndividualPointer;
    typedef Genome* GenomePointer;
    
    typedef HeterotrophProcessor* HeterotrophProcessorPointer;
    typedef HeterotrophData* HeterotrophDataPointer;

    typedef Parameters* ParametersPointer;
    typedef RandomDefault* RandomDefaultPointer;
    typedef RandomInterface* RandomInterfacePointer;

    typedef Logger* LoggerPointer;
    typedef Convertor* ConvertorPointer;
    typedef Maths* MathsPointer;

    typedef DataRecorder* DataRecorderPointer;
    typedef Datum* DatumPointer;

    typedef IntegerVectorDatum* IntegerVectorDatumPointer;
    typedef FloatVectorDatum* FloatVectorDatumPointer;
    typedef IntegerMatrixDatum* IntegerMatrixDatumPointer;
    typedef FloatMatrixDatum* FloatMatrixDatumPointer;

    typedef std::vector< IndividualPointer > IndividualArray;
    typedef std::vector< IndividualArray > IndividualMatrix;

    typedef std::vector< DatumPointer > DatumArray;

    typedef std::vector< bool > BooleanVector;
    typedef std::vector< int > IntegerVector;
    typedef std::vector< unsigned > UnsignedIntegerVector;
    typedef std::vector< float > FloatVector;
    typedef std::vector< double > DoubleVector;

    typedef std::vector< IntegerVector > IntegerMatrix;
    typedef std::vector< FloatVector > FloatMatrix;
    
    typedef std::vector< std::string > StringVector;
    typedef std::vector< StringVector > StringMatrix;


    typedef time_t Time;
    typedef timeval TimeStruct;
}

#endif
