/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef FILEWRITER
#define	FILEWRITER

#include "Environment.h"

class FileWriter {
public:
    FileWriter( );
    ~FileWriter( );
    
    void WriteOutputData( Environment& );

private:
    void InitialiseOutputDirectory( );
    bool WriteInputFiles( );
    
    bool WriteVectorDatums( );
    bool WriteMatrixDatums( );
    bool WriteStateFile( Environment& );

    std::string mOutputPath;
    std::string mDataSetDirectoryName;
};

#endif

