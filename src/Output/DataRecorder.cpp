#include "DataRecorder.h"

#include "IntegerVectorDatum.h"
#include "IntegerMatrixDatum.h"
#include "FloatVectorDatum.h"
#include "FloatMatrixDatum.h"
#include "Parameters.h"
#include "Constants.h"
#include "DateTime.h"
#include "Convertor.h"

Types::DataRecorderPointer DataRecorder::mThis = 0;

Types::DataRecorderPointer DataRecorder::Get( ) {
    if( mThis == 0 ) {
        mThis = new DataRecorder( );
    }

    return mThis;
}

DataRecorder::DataRecorder( ) {

}

DataRecorder::~DataRecorder( ) {

    if( mThis != 0 ) {
        for( unsigned int i = 0; i < mDatums.size( ); ++i ) {
            delete mDatums[ i ];
        }

        mDatums.clear( );

        delete mThis;
    }
}

void DataRecorder::AddIntegerVectorData( const unsigned int enumIndex, const std::string datumName, const int data ) {

    int datumIndex = GetDatumIndexFromName( enumIndex, datumName, Constants::eIntegerVector );

    Types::IntegerVectorDatumPointer vectorDatumPointer = ( Types::IntegerVectorDatumPointer )mDatums[ datumIndex ];
    vectorDatumPointer->AddData( data );
}

void DataRecorder::AddFloatVectorData( const unsigned int enumIndex, const std::string datumName, const float data ) {

    int datumIndex = GetDatumIndexFromName( enumIndex, datumName, Constants::eFloatVector );

    Types::FloatVectorDatumPointer vectorDatumPointer = ( Types::FloatVectorDatumPointer )mDatums[ datumIndex ];
    vectorDatumPointer->AddData( data );
}

void DataRecorder::AddFloatVectorData( const unsigned int enumIndex, const std::string datumName, const Types::FloatVector data ) {

    int datumIndex = GetDatumIndexFromName( enumIndex, datumName, Constants::eFloatVector );

    Types::FloatVectorDatumPointer vectorDatumPointer = ( Types::FloatVectorDatumPointer )mDatums[ datumIndex ];

    for( unsigned int vectorIndex = 0; vectorIndex < data.size( ); ++vectorIndex ) {
        vectorDatumPointer->AddData( data[ vectorIndex ] );
    }
}

void DataRecorder::AddFloatMatrixData( const unsigned int enumIndex, const std::string datumName, const Types::FloatVector data ) {

    int datumIndex = GetDatumIndexFromName( enumIndex, datumName, Constants::eFloatMatrix );

    Types::FloatMatrixDatumPointer matrixDatumPointer = ( Types::FloatMatrixDatumPointer )mDatums[ datumIndex ];
    matrixDatumPointer->AddData( data );
}

void DataRecorder::AddIntegerMatrixData( const unsigned int enumIndex, const std::string datumName, const Types::IntegerVector data ) {

    int datumIndex = GetDatumIndexFromName( enumIndex, datumName, Constants::eIntegerMatrix );

    Types::IntegerMatrixDatumPointer matrixDatumPointer = ( Types::IntegerMatrixDatumPointer )mDatums[ datumIndex ];
    matrixDatumPointer->AddData( data );
}

unsigned int DataRecorder::GetNumberOfDatums( ) const {
    return mDatums.size( );
}

Types::DatumPointer DataRecorder::GetDatumPointerFromIndex( const unsigned int index ) const {
    return mDatums[ index ];
}

int DataRecorder::GetDatumIndexFromName( const unsigned int enumIndex, const std::string name, const Constants::eDatumTypes type ) {

    int index = -1;

    for( unsigned int i = 0; i < mDatums.size( ); ++i ) {
        if( name == mDatums[ i ]->GetName( ) ) {
            index = i;
            break;
        }
    }

    if( index == -1 ) {

        index = mDatums.size( );

        switch( type ) {
            case Constants::eIntegerVector:
                mDatums.push_back( new IntegerVectorDatum( enumIndex, name ) );
                break;

            case Constants::eIntegerMatrix:
                mDatums.push_back( new IntegerMatrixDatum( enumIndex, name ) );
                break;

            case Constants::eFloatVector:
                mDatums.push_back( new FloatVectorDatum( enumIndex, name ) );
                break;

            case Constants::eFloatMatrix:
                mDatums.push_back( new FloatMatrixDatum( enumIndex, name ) );
                break;
        }
    }

    return index;
}
