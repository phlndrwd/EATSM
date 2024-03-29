/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef MATRIXDATUM
#define	MATRIXDATUM

#include "Constants.h"
#include "Types.h"

class MatrixDatum {
public:

    MatrixDatum( const std::string& );
    ~MatrixDatum( );
    
    void SetGroupSize( const unsigned& );
    
    float GetDataAtIndices( const unsigned&, const unsigned& ) const;
    void AddDataAtIndex( const unsigned&, const float& );
    void AddData( const Types::FloatVector );
    
    unsigned GetRows( ) const;
    unsigned GetColumns( ) const;
    const std::string& GetName( ) const;

private:
    Types::FloatMatrix mData;
    const std::string mName;
};

#endif

