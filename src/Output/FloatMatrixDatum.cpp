#include "FloatMatrixDatum.h"
#include "Parameters.h"

FloatMatrixDatum::FloatMatrixDatum( const std::string& name ) {
    mName = name;
    SetType( );
}

FloatMatrixDatum::FloatMatrixDatum( const unsigned int enumIndex, const std::string& name ) {
    mEnumIndex = enumIndex;
    mName = name;
    SetType( );
}

FloatMatrixDatum::~FloatMatrixDatum( ) {

    mName.clear( );
    for( unsigned int column = 0; column < GetColumns( ); ++column ) {
        mData[ column ].clear( );
    }
    mData.clear( );
}

void FloatMatrixDatum::SetType( ) {
    mType = Constants::eFloatMatrix;
}

void FloatMatrixDatum::AddData( const Types::FloatVector data ) {
    mData.push_back( data );
}

void FloatMatrixDatum::AddData( const int row, const int column, const float data ) {
    mData[ row ][ column ] = data;
}

const Types::FloatVector FloatMatrixDatum::GetData( const int column ) const {
    return mData[ column ];
}

float FloatMatrixDatum::GetData( const int row, const int column ) const {
    return mData[ column ][ row ];
}

unsigned int FloatMatrixDatum::GetRows( ) const {
    return mData[ GetColumns( ) - 1 ].size( );
}

unsigned int FloatMatrixDatum::GetColumns( ) const {
    return mData.size( );
}

