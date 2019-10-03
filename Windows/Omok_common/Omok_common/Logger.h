#pragma once


class Logger
{
public:
    Logger();
    ~Logger();

    static void debug( const char * fmt, ... );
};
