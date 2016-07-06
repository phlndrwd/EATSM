#ifndef LOGGER
#define LOGGER

#include "Types.h"

class Logger {
public:
    ~Logger( );
    static Types::LoggerPointer Get( );
    void LogStringNoReturn( std::string, const bool bufferMessage = false );
    void LogString( std::string, const bool bufferMessage = false );
    void LogError( std::string, const bool bufferMessage = false );
    void Click( std::string source = "" );
    void OutputRunTime( );
    
private:
    Logger( );
    
    static Types::LoggerPointer mThis;
    
    std::string mTerminalBuffer;

    Types::TimeStruct mTimeStart;
    Types::TimeStruct mTimeClick;
    
    bool mBufferFlag;
};

#endif
