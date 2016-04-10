#ifndef INCLUDED_PLATFORM_CACHE_H
#define INCLUDED_PLATFORM_CACHE_H

#include "auto_id.h"
#include "log.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "rstdint.h"
#include <boost/filesystem/path.hpp>

namespace platform {

template<typename T>
class Cache
{
public:
    typedef T CachedType;
    typedef typename boost::shared_ptr<T> Elem;
protected:
    typedef typename boost::unordered_map<int32_t, Elem> CachedValues_t;
    virtual Elem LoadImpl( const boost::filesystem::path& Source ) = 0;
private:
    CachedValues_t mCache;
    // todo: automagical cleanup
    double mLastCleanupTime;
    const int mCleanupInterval;
public:
    enum
    {
        CleanupNever = -1,
    };
    Cache( int CleanupInterval = CleanupNever );
    Elem Load( const boost::filesystem::path& Source );
};

template<typename T>
Cache<T>::Cache( int CleanupInterval )
    : mLastCleanupTime( 0.0 )
    , mCleanupInterval( CleanupInterval )
{

}

template<typename T>
typename Cache<T>::Elem Cache<T>::Load( const boost::filesystem::path& Source )
{
    int32_t Id = AutoId( Source.generic_string() );
    typename CachedValues_t::iterator It = mCache.find( Id );
    if( It != mCache.end() )
    {
        return It->second;
    }
    Elem Loaded = LoadImpl( Source );
    if( !Loaded.get() )
    {
        L1( "Warning: %s [%d] could not be loaded\n", Source.generic_string().c_str(), Id );
    }
    return mCache.emplace( Id, Loaded ).first->second;
}

} // namespace platform

#endif//INCLUDED_PLATFORM_CACHE_H
