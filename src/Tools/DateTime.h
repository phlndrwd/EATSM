#ifndef DATETIME
#define	DATETIME

#include "Types.h"

class DateTime {
public:
    ~DateTime( );
    static Types::DateTimePointer Get( );

    void StartTiming( );
    void SplitTiming( );
    void EndTiming( );

    std::string ProduceCumulativeTimeString( );
    std::string ProduceSplitTimeString( );
    std::string ProduceTotalTimeString( );
    std::string GetFormattedDateAndTime( std::string, const bool isEnd = true ) const;
    
    unsigned int GetRandomNumberSeed( ) const;
    
    unsigned int GetCumulativeTimeInSeconds( );
    unsigned int GetCumulativeTimeInMicroseconds( );

    unsigned int GetTotalTimeInSeconds( ) const;
    unsigned int GetTotalTimeInMilliseconds( ) const;

private:
    DateTime( );
    static Types::DateTimePointer mThis;

    void SetTimeNow( Types::TimeStruct* ) const;
    void CalculateTotalTimeInMilliseconds( );

    Types::TimeStruct mTimeStart;
    Types::TimeStruct mTimeSplit;
    Types::TimeStruct mTimeNow;
    Types::TimeStruct mTimeEnd;
    
    unsigned int mTotalTimeInSeconds;
    unsigned int mTotalTimeInMilliseconds;
    
    unsigned int mStartTimeAndDate;
    unsigned int mEndTimeAndDate;
};

#endif

