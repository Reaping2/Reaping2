#include "process.h"
#include <stdlib.h>
#include <algorithm>
#include <boost/predef.h>
#include <boost/filesystem.hpp>
#include <boost/assert.hpp>
#if( BOOST_OS_WINDOWS )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace platform {
namespace process {
#if( BOOST_OS_WINDOWS )
namespace buffers {
size_t const CommandLineMax = 1024;
wchar_t CommandLine[ CommandLineMax ] = { 0 };
}
namespace {
std::wstring wcstr( std::string const& str )
{
    size_t len = mbstowcs( NULL, str.c_str(), 0 );
    std::wstring wstr( len + 1, '\0' );
    mbstowcs( &wstr[0], str.c_str(), wstr.size() );
    return wstr;
}
}
bool Start( std::string const& command, std::vector<std::string> const& arguments )
{
    std::vector<std::wstring> args;
    std::transform( arguments.begin(), arguments.end(), std::back_inserter( args ), &wcstr );
    return Start( wcstr( command ), args );
}
bool Start( std::wstring const& command, std::vector<std::wstring> const& arguments )
{
    wchar_t* pt = buffers::CommandLine;
    size_t s = command.size();
    memcpy( pt, command.c_str(), s * sizeof( wchar_t ) );
    pt += s;
    *( pt++ ) = L' ';
    for( auto const& arg : arguments )
    {
        size_t s = arg.size();
        memcpy( pt, arg.c_str(), s * sizeof( wchar_t ) );
        pt += s;
        *( pt++ ) = L' ';
    }
    *( pt ) = *( pt - 1 ) = 0;

    PROCESS_INFORMATION     pi;
    STARTUPINFOW            si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t dir[1024];
    GetCurrentDirectoryW(sizeof(dir), dir);

    if (!CreateProcessW(NULL, buffers::CommandLine, NULL, NULL, FALSE, 0, NULL, dir, &si, &pi))
    {
        return false;
    }

    CloseHandle(pi.hThread);
    return true;
}
#else
namespace buffers {
size_t const MaxArgs = 32;
char* Args[MaxArgs] = { 0 };
char* GetPath()
{
    static std::string pathstr;
    if( pathstr.empty() )
    {
        pathstr = "PATH=";
        pathstr += getenv("PATH");
        pathstr += ":";
        pathstr += boost::filesystem::current_path().string();
    }
    return const_cast<char*>( pathstr.c_str() );
}
char* Env[] = { GetPath(), NULL };
}
bool Start( std::string const& command, std::vector<std::string> const& arguments )
{
    size_t ctr = 0;
    buffers::Args[ ctr++ ] = const_cast<char*>( command.c_str() );
    for( auto const& arg : arguments )
    {
        buffers::Args[ ctr++ ] = const_cast<char*>( arg.c_str() );
    }
    buffers::Args[ ctr ] = NULL;

    pid_t pid = fork();

    if (pid == -1)
    {
        return false;
    }
    else if (pid == 0) {
        (void) execvpe( command.c_str(),
                buffers::Args, buffers::Env );
        exit(1);
    }
    return true;
}
namespace {
std::string mbstr( std::wstring const& wstr )
{
    size_t len = wcstombs( NULL, wstr.c_str(), 0 );
    std::string str( len + 1, '\0' );
    wcstombs( &str[0], wstr.c_str(), str.size() );
    return str;
}
}
bool Start( std::wstring const& command, std::vector<std::wstring> const& arguments )
{
    std::vector<std::string> args;
    std::transform( arguments.begin(), arguments.end(), std::back_inserter( args ), &mbstr );
    return Start( mbstr( command ), args );
}
#endif
}
}


