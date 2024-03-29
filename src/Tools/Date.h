/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATE
#define	DATE

#include <sys/time.h>   // For struct timeval
#include "Constants.h"

class Date {
public:
    static std::string GetDateAndTimeString( const std::string format = Constants::cCompleteDateFormat, unsigned addedSeconds = 0 );
};

#endif

