#include "IntegerMatrixDatum.h"
#include "Parameters.h"

IntegerMatrixDatum::IntegerMatrixDatum( const std::string& name ) {
    mName = name;
    SetType( );
}

IntegerMatrixDatum::IntegerMatrixDatum( const unsigned int enumIndex, const std::string& name ) {
    mEnumIndex = enumIndex;
    mName = name;
    SetType( );
}

IntegerMatrixDatum::~IntegerMatrixDatum( ) {

    mName.clear( );
    for( unsigned int column = 0; column < GetColumns( ); ++column ) {
        mData[ column ].clear( );
    }
    mData.clear( );
}

void IntegerMatrixDatum::SetType( ) {
    mType = Constants::eIntegerMatrix;
}

void IntegerMatrixDatum::AddData( const Types::IntegerVector data ) {
    mData.push_back( data );
}

void IntegerMatrixDatum::AddData( const int row, const int column, const int data ) {
    mData[ row ][ column ] = data;
}

const Types::IntegerVector IntegerMatrixDatum::GetData( const int column ) const {
    return mData[ column ];
}

int IntegerMatrixDatum::GetData( const int row, const int column ) const {
    return mData[ column ][ row ];
}

unsigned int IntegerMatrixDatum::GetRows( ) const {
    return mData[ GetColumns( ) - 1 ].size( );
}

unsigned int IntegerMatrixDatum::GetColumns( ) const {
    return mData.size( );
}


