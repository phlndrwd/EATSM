#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <sstream>      // For std::stringstream
#include <fstream>      // For std::ofstream
#include <sys/time.h>   // For struct timeval

class Environment;
class Nutrient;
class Phytoplankton;
class Heterotrophs;
class Individual;
class Genome;

class HeterotrophData;
class HeterotrophProcessor;

class Parameters;
class RandomDefault;
class RandomInterface;
class Logger;
class DateTime;
class Convertor;
class Maths;
class FileReader;
class FileWriter;
class DataRecorder;
class Datum;
class RestartState;
class IntegerVectorDatum;
class FloatVectorDatum;
class IntegerMatrixDatum;
class FloatMatrixDatum;

namespace Types {

    typedef Environment* EnvironmentPointer;
    typedef Nutrient* NutrientPointer;
    typedef Phytoplankton* PhytoplanktonPointer;
    typedef Heterotrophs* HeterotrophsPointer;
    typedef Individual* IndividualPointer;
    typedef Genome* GenomePointer;
    
    typedef HeterotrophProcessor* HeterotrophProcessorPointer;
    typedef HeterotrophData* HeterotrophDataPointer;

    typedef Parameters* ParametersPointer;
    typedef RandomDefault* RandomDefaultPointer;
    typedef RandomInterface* RandomInterfacePointer;

    typedef Logger* LoggerPointer;
    typedef DateTime* DateTimePointer;
    typedef Convertor* ConvertorPointer;
    typedef Maths* MathPointer;

    typedef FileReader* FileReaderPointer;
    typedef FileWriter* FileWriterPointer;
    typedef DataRecorder* DataRecorderPointer;
    typedef Datum* DatumPointer;
    typedef RestartState* RestartStatePointer;

    typedef IntegerVectorDatum* IntegerVectorDatumPointer;
    typedef FloatVectorDatum* FloatVectorDatumPointer;
    typedef IntegerMatrixDatum* IntegerMatrixDatumPointer;
    typedef FloatMatrixDatum* FloatMatrixDatumPointer;

    typedef std::vector< IndividualPointer > IndividualArray;
    typedef std::vector< IndividualArray > IndividualMatrix;

    typedef std::vector< DatumPointer > DatumArray;

    typedef std::vector< bool > BooleanVector;
    typedef std::vector< int > IntegerVector;
    typedef std::vector< unsigned int > UnsignedIntegerVector;
    typedef std::vector< float > FloatVector;
    typedef std::vector< double > DoubleVector;

    typedef std::vector< IntegerVector > IntegerMatrix;
    typedef std::vector< FloatVector > FloatMatrix;
    
    typedef std::vector< std::string > StringVector;

    typedef std::ifstream InputFileStream;
    typedef std::ofstream OutputFileStream;
    typedef std::stringstream StringStream;


    typedef time_t Time;
    typedef timeval TimeStruct;
}

#endif
