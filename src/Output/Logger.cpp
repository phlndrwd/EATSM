#include "Logger.h"
#include "DateTime.h"
#include "Parameters.h"
#include "FileWriter.h"

#include <iostream>

Types::LoggerPointer Logger::mThis = 0;

Types::LoggerPointer Logger::Get( ) {
    if( mThis == 0 ) {
        mThis = new Logger( );
    }
    return mThis;
}

Logger::~Logger( ) {

    if( mThis != 0 ) {
        mTerminalBuffer.clear( );
        delete mThis;
    }
}

Logger::Logger( ) {

}

void Logger::LogStringNoReturn( std::string message, const bool bufferMessage ) {

    if( bufferMessage == false ) {
        if( mBufferFlag == true ) {
           FileWriter::Get( )->LogTerminalToFile( mTerminalBuffer );
        }
        FileWriter::Get( )->LogTerminalToFile( message );
        mBufferFlag = false;
        
    } else {
        mTerminalBuffer.append( message );
        mBufferFlag = true;
    }

    std::cout << message;
}

void Logger::LogString( std::string message, const bool bufferMessage ) {

    message.append( "\n" );
    LogStringNoReturn( message, bufferMessage );
}

void Logger::LogError( std::string message, const bool bufferMessage ) {

    message.insert( 0, "ERROR> " );
    message.append( "\n" );

    LogStringNoReturn( message, bufferMessage );
}

void Logger::Click( std::string source ) {
    LogString( source + DateTime::Get( )->ProduceSplitTimeString( ) );
}

void Logger::OutputRunTime( ) {
    LogString( DateTime::Get( )->ProduceTotalTimeString( ) );
}
