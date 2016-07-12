#include "MatrixDatum.h"

MatrixDatum::MatrixDatum( const std::string& name ) : mName( name ) {
    
}

MatrixDatum::~MatrixDatum( ) {

}

float MatrixDatum::GetDataAtIndices( const unsigned& rowIndex, const unsigned& columnIndex ) const {
    return mData[ columnIndex ][ rowIndex ];
}

void MatrixDatum::AddData( const Types::FloatVector data ) {
    mData.push_back( data );
}

unsigned MatrixDatum::GetRows( ) const {
    return mData[ 0 ].size( );
}

unsigned MatrixDatum::GetColumns( ) const {
    return mData.size( );
}

const std::string& MatrixDatum::GetName( ) const {
    return mName;
}
