#include "Timer.h"

Timer::Timer( bool start ) {
    if( start == true ) Start( );
}

Timer::~Timer( ) {
}

void Timer::Start( ) {
    mStartTime = std::chrono::high_resolution_clock::now( );
    mSplitTime = mStartTime;
}

double Timer::Elapsed( ) {
    
    mSplitTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > elapsed;

    elapsed = mSplitTime - mStartTime;
    
    return elapsed.count( );
}

double Timer::Split( ) {
    
    std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > split;

    split = timeNow - mSplitTime;
    mSplitTime = timeNow;
    
    return split.count( );
}

double Timer::Stop( ) {
    
    mStopTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > total;

    total = mStopTime - mStartTime;

    return total.count( );
}