#include "filesystem.h"
#include "ifile.h"
#include "ipackage.h"

#include <boost/ptr_container/ptr_map.hpp>
#include "rstdint.h"
#include <boost/thread/mutex.hpp>


namespace platform {
namespace detail {

class FilesysImpl
{
    typedef boost::ptr_map<int32_t, IPackage> PackageMap_t;
    PackageMap_t mPackages;
    int32_t mNextPrio;  //todo
    boost::mutex mOpenMutex;
public:
    FilesysImpl();
    void Mount( std::auto_ptr<IPackage> Pack );
    std::auto_ptr<File> Open( const boost::filesystem::path& Path );
    void GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir = boost::filesystem::path() );
};

void FilesysImpl::Mount( std::auto_ptr<IPackage> Pack )
{
    boost::mutex::scoped_lock Lock( mOpenMutex );
    //todo: priority?
    ++mNextPrio;
    mPackages.insert( mNextPrio, Pack.release() );
}

FilesysImpl::FilesysImpl()
    : mNextPrio( 0 )
{

}

std::auto_ptr<File> FilesysImpl::Open( const boost::filesystem::path& Path )
{
    boost::mutex::scoped_lock Lock( mOpenMutex );
    std::auto_ptr<File> Ret;
    for( PackageMap_t::iterator i = mPackages.begin(), e = mPackages.end(); i != e && !Ret.get(); ++i )
    {
        Ret = std::auto_ptr<File>( i->second->Open( Path ).release() );
    }
    return Ret;
}

void FilesysImpl::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir/*=boost::filesystem::path()*/ )
{
    boost::mutex::scoped_lock Lock( mOpenMutex );
    for( PackageMap_t::iterator i = mPackages.begin(), e = mPackages.end(); i != e; ++i )
    {
        i->second->GetFileNames( Paths, Dir );
    }
}

} // namespace detail

Filesys::Filesys()
{
    mImpl.reset( new detail::FilesysImpl );
}

Filesys::~Filesys()
{
}

void Filesys::Mount( std::auto_ptr<IPackage> Pack )
{
    mImpl->Mount( Pack );
}

void Filesys::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir )
{
    mImpl->GetFileNames( Paths, Dir );
}

std::auto_ptr<File> Filesys::Open( boost::filesystem::path const& Path )
{
    return mImpl->Open( Path );
}

} // namespace platform
