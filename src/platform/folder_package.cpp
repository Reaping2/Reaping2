#include "folder_package.h"

#include "osfile.h"
#include "log.h"

#include <algorithm>
#include <iterator>

namespace platform {
namespace {
bool contains( boost::filesystem::path const& container, boost::filesystem::path const& contained )
{
    auto containerlen = std::distance( container.begin(), container.end() );
    auto containedlen = std::distance( contained.begin(), contained.end() );
    if( containedlen <= containerlen )
    {
        return false;
    }
    return std::equal( container.begin(), container.end(), contained.begin() );
}
}
class FolderPackage::Impl
{
    boost::filesystem::path mRootPath;
public:
    Impl( boost::filesystem::path const& root );
    std::unique_ptr<File> Open( boost::filesystem::path const& Path );
    void GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir = boost::filesystem::path() );
};

std::unique_ptr<File> FolderPackage::Impl::Open( const boost::filesystem::path& Path )
{
    // check
    boost::system::error_code ec;
    boost::filesystem::path full = boost::filesystem::canonical( Path, mRootPath, ec );
    if( !!ec || !contains( mRootPath, full ) )
    {
        return std::unique_ptr<File>( nullptr );
    }

    std::unique_ptr<File> F( new OsFile( full ) );
    return F;
}

void FolderPackage::Impl::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir )
{
    boost::system::error_code ec;
    boost::filesystem::path full = boost::filesystem::canonical( Dir, mRootPath, ec );
    if( !!ec || !contains( mRootPath, full ) )
    {
        return;
    }
    boost::filesystem::recursive_directory_iterator iter( full ), eod;
    for( ; iter != eod; ++iter )
    {
        auto const& p = *iter;
        if( is_regular_file( p ) )
        {
            Paths.push_back( p );
        }
    }
}

FolderPackage::Impl::Impl( boost::filesystem::path const& root )
    : mRootPath( root )
{

}

FolderPackage::FolderPackage( boost::filesystem::path const& path )
{
    boost::system::error_code ec;
    boost::filesystem::path full = boost::filesystem::canonical( path, ec );
    if( !ec )
    {
        mImpl.reset( new Impl( full ) );
    }
    else
    {
        L1( "Specified path for FolderPackage is invalid!" );
    }
}

FolderPackage::~FolderPackage()
{
}

std::unique_ptr<File> FolderPackage::Open( boost::filesystem::path const& path )
{
    return !!mImpl ? std::move( mImpl->Open( path ) ) : std::unique_ptr<File>( nullptr );
}

uint32_t FolderPackage::Checksum() const
{
    L1( "FolderPackage::Checksum() not implemented, returning 0" );
    return 0;
}

void FolderPackage::GetFileNames( PathVect_t& paths, boost::filesystem::path const& dir )
{
    if( !mImpl )
    {
        return;
    }
    mImpl->GetFileNames( paths, dir );
}

} // namespace platform

