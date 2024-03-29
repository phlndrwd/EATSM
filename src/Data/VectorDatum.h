/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef VECTORDATUM
#define	VECTORDATUM

#include "Constants.h"
#include "Types.h"

class VectorDatum {
public:
    VectorDatum( const std::string& );
    ~VectorDatum( );
    
    float GetDataAtIndex( const unsigned& ) const;
    
    void SetData( const Types::FloatVector );
    void AddData( const float& );
    
    unsigned GetSize( ) const;
    const std::string& GetName( ) const;

private:
    Types::FloatVector mData;
    std::string mName;
};

#endif

