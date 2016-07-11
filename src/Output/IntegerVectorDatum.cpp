#include "IntegerVectorDatum.h"

IntegerVectorDatum::IntegerVectorDatum( const std::string& name ) {
    mName = name;
    SetType( );
}

IntegerVectorDatum::IntegerVectorDatum( unsigned enumIndex, const std::string& name ) {
    mEnumIndex = enumIndex;
    mName = name;
    SetType( );
}

IntegerVectorDatum::~IntegerVectorDatum( ) {
    mName.clear( );
    mData.clear( );
}

void IntegerVectorDatum::SetType( ) {
    mType = Constants::eIntegerVector;
}

void IntegerVectorDatum::AddData( const int data ) {
    mData.push_back( data );
}

int IntegerVectorDatum::GetData( const int index ) const {
    return mData[ index ];
}

unsigned IntegerVectorDatum::GetLength( ) const {
    return mData.size( );
}
