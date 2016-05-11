/*
 * Copyright (c) 2015 Hideki Shiro
 */

#ifndef logger_h
#define logger_h

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
};

class LoggerClass {
public:
    static void begin(LogLevel level, int speed);
    static void trace(const char *message);
    static void debug(const char *message);
    static void info(const char *message);
    static void warn(const char *message);
    static void error(const char *message);
private:
    static LogLevel level;
    static void println(LogLevel level, const char *message);
};

extern LoggerClass Logger;

#endif
