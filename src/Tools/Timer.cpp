#include "Timer.h"

Timer::Timer( ) {
}

Timer::~Timer( ) {
}

void Timer::Start( ) {
    mStartTime = std::chrono::high_resolution_clock::now( );
}

double Timer::Split( ) {
    
    mSplitTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > elapsed;

    elapsed = mSplitTime - mStartTime;

    return elapsed.count( );
}

double Timer::Stop( ) {
    
    mStopTime = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > elapsed;

    elapsed = mStopTime - mStartTime;

    return elapsed.count( );
}