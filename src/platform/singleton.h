#ifndef INCLUDED_PLATFORM_SINGLETON_H
#define INCLUDED_PLATFORM_SINGLETON_H

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

namespace platform {

// TODO: migrate from singleton to intf/impl
template<typename T>
class Singleton
{
public:
    static T& Get()
    {
        BOOST_ASSERT( !is_destructed );
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

} // namespace platform

#endif//INCLUDED_PLATFORM_SINGLETON_H
