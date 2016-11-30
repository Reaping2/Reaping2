#include "log.h"
#include "jsonreader.h"
#include "ifile.h"
#include "osfile.h"
#include <cstdio>
#include <cstdarg>
#include <memory>
#include <iosfwd>
#include "GLFW/glfw3.h"

namespace platform {

void Logger::Log( int Level, char const* format, ... )
{
    if( mDisabledLevels & ( 1 << Level )||mLogFile.mFile==nullptr )
    {
        return;
    }
    va_list arg;
    int done;
    fprintf( mLogFile.mFile, "[%f]", glfwGetTime() );
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
    : mDisabledLevels( 0 )
    , mLogFile()
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
    const size_t NumDisabledLogs = DisabledLogs.size();
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
