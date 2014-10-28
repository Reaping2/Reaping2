#ifndef INCLUDED_PLATFORM_SINGLETON_H
#define INCLUDED_PLATFORM_SINGLETON_H

#include <cassert>
#include <boost/thread/mutex.hpp>

template<typename T>
class Singleton
{
public:
    static T& Get()
    {
        assert( !is_destructed );
        ( void )is_destructed; // prevent removing is_destructed in Release configuration

        boost::mutex::scoped_lock lock( GetMutex() );
        static T instance;
        return instance;
    }
    Singleton() {}
    virtual ~Singleton()
    {
        is_destructed = true;
    }
private:
    static bool is_destructed;

    static boost::mutex& GetMutex()
    {
        static boost::mutex mutex;
        return mutex;
    }
};

// force creating mutex before main() is called
template<typename T>
bool Singleton<T>::is_destructed = ( Singleton<T>::GetMutex(), false );

#endif//INCLUDED_PLATFORM_SINGLETON_H