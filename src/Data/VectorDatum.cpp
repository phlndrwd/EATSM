/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "VectorDatum.h"

VectorDatum::VectorDatum( const std::string& name ) {
    mName = name;
}

VectorDatum::~VectorDatum( ) {

}

float VectorDatum::GetDataAtIndex( const unsigned& index ) const {
    return mData[ index ];
}

void VectorDatum::SetData( const Types::FloatVector data ) {
    mData = data;
}

void VectorDatum::AddData( const float& data ) {
    mData.push_back( data );
}
unsigned VectorDatum::GetSize( ) const {
    return mData.size( );
}

const std::string& VectorDatum::GetName( ) const {
    return mName;
}
