#include "stdafx.h"
#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>


Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::debug( const char * fmt, ... )
{
    va_list arg;
    va_start( arg, fmt );

    char cbuf[512];
    char final[1024];
    vsprintf_s( cbuf, fmt, arg );

    wsprintf( final, ">>> [DEBUG] %s\n", cbuf );

    va_end( arg );

    OutputDebugString( final );
}
