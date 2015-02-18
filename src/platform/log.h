#ifndef INCLUDED_PLATFORM_LOG_H
#define INCLUDED_PLATFORM_LOG_H

#include "singleton.h"
#include <stdio.h>

namespace platform {

struct AutoNormalFile
{
    FILE* mFile;
    AutoNormalFile(const char* name, const char* mode);
    ~AutoNormalFile();
};

class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;
    AutoNormalFile mLogFile;
    Logger();
    size_t mDisabledLevels;
public:
    void Log( int Level, char const* format, ... );
};

} // namespace platform

#define LOGGING_ENABLED
#ifdef LOGGING_ENABLED
#define LOG(...) platform::Logger::Get().Log(0,__VA_ARGS__)
#define L1(...) platform::Logger::Get().Log(1,__VA_ARGS__)
#define L2(...) platform::Logger::Get().Log(2,__VA_ARGS__)
#else
#define LOG(...) (void)(__VA_ARGS__)
#define L1(...) (void)(__VA_ARGS__)
#define L2(...) (void)(__VA_ARGS__)
#endif//LOGGING_ENABLED

#endif//INCLUDED_PLATFORM_LOG_H
