#include "FloatVectorDatum.h"

FloatVectorDatum::FloatVectorDatum( const std::string& name ) {
    mName = name;
    SetType( );
}

FloatVectorDatum::FloatVectorDatum( const unsigned int enumIndex, const std::string& name ) {
    mEnumIndex = enumIndex;
    mName = name;
    SetType( );
}

FloatVectorDatum::~FloatVectorDatum( ) {
    mName.clear( );
    mData.clear( );
}

void FloatVectorDatum::SetType( ) {
    mType = Constants::eFloatVector;
}

void FloatVectorDatum::AddData( const float data ) {
    mData.push_back( data );
}

float FloatVectorDatum::GetData( const int index ) const {
    return mData[ index ];
}

unsigned int FloatVectorDatum::GetLength( ) const {
    return mData.size( );
}

