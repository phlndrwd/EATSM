/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model (EATSM)                       *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Date.h"

#include "Constants.h"
#include "Parameters.h"

std::string Date::GetDateAndTimeString( const std::string format, unsigned addedSeconds ) {
    char dateTimeChar[ Constants::cDateTimeBufferSize ];
    timeval timeNow;
    gettimeofday( &timeNow, nullptr );
    time_t rawtime = static_cast < time_t > ( timeNow.tv_sec + addedSeconds );
    
    struct tm *timeinfo;
    timeinfo = localtime( &rawtime );

    strftime( dateTimeChar, Constants::cDateTimeBufferSize, format.c_str( ), timeinfo );
    std::string dateTime( dateTimeChar );

    return dateTime;
}
