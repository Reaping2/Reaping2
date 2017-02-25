#include "log.h"
#include "jsonreader.h"
#include "ifile.h"
#include "osfile.h"
#include <cstdio>
#include <cstdarg>
#include <memory>
#include <iosfwd>
#include <boost/format.hpp>
#include "game_clock.h"

namespace platform {

LogEntryDesc LogEntryDesc::create( int l, std::string const& ss, int li, std::string const& f, std::string const& fu )
{
    LogEntryDesc desc;
    desc.level = l;
    desc.subsystem = ss;
    desc.line = li;
    desc.file = f;
    desc.function = fu;
    desc.timestamp = Clock::Now();
    return desc;
}

LogEntry::LogEntry( LogEntryDesc const& desc )
    : mDesc( desc )
{
}

LogEntry::~LogEntry()
{
    Logger::Get().Log( *this );
}

LogEntryDesc const& LogEntry::desc() const
{
    return mDesc;
}

std::string LogEntry::print() const
{
    return boost::str( boost::format( "%1% %2% %3% %4% %5% [%6%] : %7%\n" ) % mDesc.timestamp % mDesc.subsystem % mDesc.level % mDesc.file % mDesc.line % mDesc.function % mContent.str() );
}

void Logger::Log( LogEntry const& entry )
{
    if( mDisabledLevels & ( 1 << entry.desc().level ) || mLogFile.mFile == nullptr )
    {
        return;
    }
    std::string const& log = entry.print();
    fprintf( mLogFile.mFile, "%s", log.c_str() );
}

void Logger::Log( int Level, char const* format, ... )
{
    if( mDisabledLevels & ( 1 << Level )||mLogFile.mFile==nullptr )
    {
        return;
    }
    va_list arg;
    int done;
    fprintf( mLogFile.mFile, "[%f]", Clock::Now() );
    va_start ( arg, format );
    done = vfprintf ( mLogFile.mFile, format, arg );
#if BOOST_OS_WINDOWS 
    // Temp. Currently using va_args two times does crash using gcc.
    if(Level==1) done = vfprintf ( stderr, format, arg );
#endif
    fflush( mLogFile.mFile );
    va_end ( arg );
}

Logger::Logger()
    : mLogFile()
    , mDisabledLevels( 0 )
{
    std::auto_ptr<File> f( new OsFile( "debug.json" ) );
    if( !f.get() || !f->IsValid() )
    {
        return;
    }
    JsonReader Reader( *f.get() );
    if( !Reader.IsValid() )
    {
        return;
    }
    Json::Value& Root = Reader.GetRoot();
    if( !Root.isObject() )
    {
        return;
    }
    Json::Value& DisabledLogs = Root["disabled_logs"];
    if( !DisabledLogs.isArray() )
    {
        return;
    }
    for( Json::Value::iterator it = DisabledLogs.begin(), e = DisabledLogs.end(); it != e; ++it )
    {
        Json::Value const& DisLog = *it;
        uint32_t Val;
        if( !Json::GetUInt( DisLog, Val ) )
        {
            continue;
        }
        mDisabledLevels |= ( 1 << Val );
    }
}


void Logger::SetFileName( std::string filename )
{
    if (mLogFile.mFile)
    {
        fclose( mLogFile.mFile );
    }
    mLogFile.mFile = fopen( filename.c_str(), "w" );
}

AutoNormalFile::AutoNormalFile( const char* name, const char* mode )
{
    mFile = fopen ( name, mode );
}


AutoNormalFile::AutoNormalFile()
{

}

AutoNormalFile::~AutoNormalFile()
{
    if (mFile)
    {
        fclose( mFile );
    }
}

} // namespace platform
