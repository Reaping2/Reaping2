#ifndef INCLUDED_PLATFORM_SINGLETON_H
#define INCLUDED_PLATFORM_SINGLETON_H

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>
#include <cstdlib>

namespace platform {

// TODO: migrate from singleton to intf/impl
template<typename T>
class Singleton
{
public:
    static T& Get()
    {
        static T instance;
        return instance;
    }
    Singleton() {}
    virtual ~Singleton() {}
};

} // namespace platform

#endif//INCLUDED_PLATFORM_SINGLETON_H
