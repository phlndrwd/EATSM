/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "MatrixDatum.h"

MatrixDatum::MatrixDatum( const std::string& name ): mName( name ) {

}

MatrixDatum::~MatrixDatum( ) {

}

void MatrixDatum::SetGroupSize( const unsigned& size ) {
    mData.resize( size );
}

float MatrixDatum::GetDataAtIndices( const unsigned& rowIndex, const unsigned& columnIndex ) const {
    return mData[ columnIndex ][ rowIndex ];
}

void MatrixDatum::AddDataAtIndex( const unsigned& index, const float& data ) {
    mData[ index ].push_back( data );
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
