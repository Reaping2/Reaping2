#ifndef INCLUDED_PLATFORM_LOG_H
#define INCLUDED_PLATFORM_LOG_H

class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;
    Logger();
    size_t mDisabledLevels;
public:
    void Log( int Level, char const* format, ... );
};

#define LOGGING_ENABLED
#ifdef LOGGING_ENABLED
#define LOG(...) Logger::Get().Log(0,__VA_ARGS__)
#define L1(...) Logger::Get().Log(1,__VA_ARGS__)
#define L2(...) Logger::Get().Log(2,__VA_ARGS__)
#else
#define LOG(...) (void)(__VA_ARGS__)
#define L1(...) (void)(__VA_ARGS__)
#define L2(...) (void)(__VA_ARGS__)
#endif//LOGGING_ENABLED

#endif//INCLUDED_PLATFORM_LOG_H
