#ifndef INCLUDED_PLATFORM_LOG_H
#define INCLUDED_PLATFORM_LOG_H

#include "singleton.h"
#include <stdio.h>
#include <boost/current_function.hpp>

namespace platform {

struct AutoNormalFile
{
    FILE* mFile = nullptr;
    AutoNormalFile( const char* name, const char* mode );
    AutoNormalFile();
    ~AutoNormalFile();
};

class LogEntry;
class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;
    AutoNormalFile mLogFile;
    Logger();
    size_t mDisabledLevels;
public:
    void Log( int Level, char const* format, ... );
    void Log( LogEntry const& entry );
    void SetFileName( std::string filename );
};

struct LogEntryDesc
{
    int level;
    std::string subsystem;
    int line;
    std::string file;
    std::string function;
    double timestamp;
    static LogEntryDesc create( int level, std::string const& subsystem, int line, std::string const& file, std::string const& function );
};

class LogEntry
{
    std::stringstream mContent;
    LogEntryDesc mDesc;
public:
    template<typename T>
    LogEntry& operator << ( T const& t )
    {
        mContent << t;
        return *this;
    }
    LogEntry( LogEntryDesc const& desc );
    std::string print() const;
    LogEntryDesc const& desc() const;
    ~LogEntry();
};

} // namespace platform

#define LOG(...) platform::Logger::Get().Log(0,__VA_ARGS__)
#define L1(...) platform::Logger::Get().Log(1,__VA_ARGS__)
#define L2(...) platform::Logger::Get().Log(2,__VA_ARGS__)
#define LS( system, level ) LogEntry( LogEntryDesc::create( level, system, __LINE__, __FILE__, BOOST_CURRENT_FUNCTION ) )
#define LL() LS( "undefined", 1 )

#endif//INCLUDED_PLATFORM_LOG_H
