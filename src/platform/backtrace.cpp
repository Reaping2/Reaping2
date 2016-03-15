#include "backtrace.h"
#include <iostream>
#include <boost/predef.h>
#include <csignal>
#include <exception>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <fstream>

#if BOOST_OS_WINDOWS
#include <dbg/debugger.hpp>
#include <dbg/symbols.hpp>
#include <dbg/frames.hpp>
#else
#define UNW_LOCAL_ONLY
#include <stdlib.h>
#include <unistd.h>
#include <libunwind.h>
#include <stdio.h>
#include <memory>
#endif

namespace {
#if BOOST_OS_WINDOWS
void bt_impl( std::ostream& strm )
{
    dbg::symdb db;
    dbg::call_stack< 64 > traceback;
    traceback.collect( 0 );
    traceback.log( db, strm );
}
#else
struct elem {
    char mMangledName[256];
    long mIp;
    long mSp;
    elem( unw_cursor_t& cursor )
    {
        unw_word_t ip, sp, offp;
        mMangledName[0] = 0;
        unw_get_proc_name (&cursor, mMangledName, 256, &offp);
        unw_get_reg (&cursor, UNW_REG_IP, &ip);
        unw_get_reg (&cursor, UNW_REG_SP, &sp);
        mIp = ip;
        mSp = sp;
    }
};
std::vector<elem> gtraceback;
void bt_impl( std::ostream& strm )
{
    unw_cursor_t cursor;
    unw_context_t uc;

    unw_getcontext (&uc);
    unw_init_local (&cursor, &uc);

    gtraceback.clear();
    while (unw_step(&cursor) > 0)
    {
        gtraceback.emplace_back( cursor );
    }
    for( auto const& i : gtraceback )
    {
        strm << i.mMangledName << " ip = " << std::hex << i.mIp << " sp = " << i.mSp << std::dec << "\n";
    }
}
#endif

void saveBacktrace()
{
    // kind of ugly, prevent additional bts generated after incorrect termination ( registered_id asserts, etc )
    static bool handled = false;
    if( handled )
    {
        return;
    }
    handled = true;
    std::string const& tempmodel = boost::filesystem::temp_directory_path().string();
    boost::filesystem::path temp = boost::filesystem::unique_path( tempmodel + "/reaping2-\%\%\%\%-\%\%\%\%-\%\%\%\%-\%\%\%\%.txt" );
    std::string tempstr = temp.string();
    std::ofstream ofs( tempstr.c_str() );
    bt_impl( ofs );
}

void handle( int signal )
{
    saveBacktrace();
    abort();
}

void handle_exc()
{
    saveBacktrace();
    abort();
}

void subscribe()
{
    std::signal( SIGABRT, &handle );
    std::signal( SIGFPE, &handle );
    std::signal( SIGILL, &handle );
    std::signal( SIGSEGV, &handle );
    std::set_terminate( &handle_exc );
}

struct CrashListener {
    CrashListener()
    {
        subscribe();
    }
} gCrashListener;
}

namespace platform {
std::ostream& backtrace( std::ostream& strm )
{
    bt_impl( strm );
    return strm;
}
}

FORCE_STATIC_INIT_IMPLEMENT( backtrace )

