#include "DataRecorder.h"
#include "Parameters.h"
#include "Constants.h"
#include "Convertor.h"
#include "VectorDatum.h"
#include "MatrixDatum.h"

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
    if( mThis != NULL ) {
        delete mThis;
    }
    for( Types::VectorDatumMap::iterator iter = mVectorDatumMap.begin( ); iter != mVectorDatumMap.end( ); ++iter ) {
        delete iter->second;
    }
    for( Types::MatrixDatumMap::iterator iter = mMatrixDatumMap.begin( ); iter != mMatrixDatumMap.end( ); ++iter ) {
        delete iter->second;
    }
}

bool DataRecorder::Initialise( const Types::StringMatrix& rawOutputParameterData ) {
    if( rawOutputParameterData.size( ) > 0 ) {
        for( unsigned rowIndex = 0; rowIndex < rawOutputParameterData.size( ); ++rowIndex ) {
            std::string name = Convertor::Get( )->RemoveWhiteSpace( rawOutputParameterData[ rowIndex ][ Constants::eDatumName ] );
            std::string type = Convertor::Get( )->RemoveWhiteSpace( Convertor::Get( )->ToLowercase( rawOutputParameterData[ rowIndex ][ Constants::eDatumType ] ) );

            Types::StringVector datumMetadata;
            datumMetadata.push_back( name );
            datumMetadata.push_back( type );

            if( type == Constants::cVectorDatumTypeName ) {
                mVectorDatumMetadata.push_back( datumMetadata );
            } else if( type == Constants::cMatrixDatumTypeName ) {
                mMatrixDatumMetadata.push_back( datumMetadata );
            }
        }
        return true;
    } else {
        return false;
    }
}

void DataRecorder::AddDataTo( const std::string& name, const float& data ) {
    Types::VectorDatumPointer vectorDatum = GetVectorDatumFromName( name );

    if( vectorDatum != NULL ) {
        vectorDatum->AddData( data );
    }
}

void DataRecorder::SetVectorDataOn( const std::string& name, const Types::FloatVector data ) {
    Types::VectorDatumPointer vectorDatum = GetVectorDatumFromName( name );

    if( vectorDatum != NULL ) {
        vectorDatum->SetData( data );
    }
}

void DataRecorder::AddDataTo( const std::string& name, const Types::FloatVector data ) {
    Types::MatrixDatumPointer matrixDatum = GetMatrixDatumFromName( name );

    if( matrixDatum != NULL ) {
        matrixDatum->AddData( data );
    }
}

void DataRecorder::AddInputFilePath( const std::string& inputFilePath ) {
    mInputFilePaths.push_back( inputFilePath );
}

Types::VectorDatumMap DataRecorder::GetVectorDatumMap( ) const {
    return mVectorDatumMap;
}

Types::MatrixDatumMap DataRecorder::GetMatrixDatumMap( ) const {
    return mMatrixDatumMap;
}

Types::StringVector DataRecorder::GetInputFilePaths( ) const {
    return mInputFilePaths;
}

Types::VectorDatumPointer DataRecorder::GetVectorDatumFromName( const std::string& name ) {
    Types::VectorDatumPointer vectorDatum = NULL;
    Types::VectorDatumMap::iterator iter = mVectorDatumMap.find( name );

    if( iter != mVectorDatumMap.end( ) ) {
        vectorDatum = iter->second;
    } else {
        for( unsigned datumIndex = 0; datumIndex < mVectorDatumMetadata.size( ); ++datumIndex ) {
            std::string datumName = mVectorDatumMetadata[ datumIndex ][ Constants::eDatumName ];
            
            if( Convertor::Get( )->ToLowercase( datumName ) == Convertor::Get( )->ToLowercase( name ) ) {
                vectorDatum = new VectorDatum( datumName );
                mVectorDatumMap.insert( std::pair< std::string, Types::VectorDatumPointer >( datumName, vectorDatum ) );
                break;
            }
        }
    }
    return vectorDatum;
}

Types::MatrixDatumPointer DataRecorder::GetMatrixDatumFromName( const std::string& name ) {
    Types::MatrixDatumPointer matrixDatum = NULL;
    Types::MatrixDatumMap::iterator iter = mMatrixDatumMap.find( name );

    if( iter != mMatrixDatumMap.end( ) ) {
        matrixDatum = iter->second;
    } else {
        for( unsigned datumIndex = 0; datumIndex < mMatrixDatumMetadata.size( ); ++datumIndex ) {
            std::string datumName = mMatrixDatumMetadata[ datumIndex ][ Constants::eDatumName ];

            if( Convertor::Get( )->ToLowercase( datumName ) == Convertor::Get( )->ToLowercase( name ) ) {
                matrixDatum = new MatrixDatum( datumName );
                mMatrixDatumMap.insert( std::pair< std::string, Types::MatrixDatumPointer >( datumName, matrixDatum ) );
                break;
            }
        }
    }
    return matrixDatum;
}