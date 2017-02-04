#ifndef INCLUDED_PLATFORM_PACKAGE_H
#define INCLUDED_PLATFORM_PACKAGE_H

#include "ipackage.h"

namespace platform {

namespace detail {
class PackageImpl;
} // namespace detail

class Package : public IPackage
{
public:
    Package( std::auto_ptr<File> source );
    ~Package();
    std::unique_ptr<File> Open( boost::filesystem::path const& path );
    void GetFileNames( PathVect_t& paths, boost::filesystem::path const& dir = boost::filesystem::path() );
    uint32_t Checksum() const;
private:
    std::auto_ptr<detail::PackageImpl> mImpl;
};

class PackageWriter
{
public:
    PackageWriter( std::auto_ptr<File> target );
    ~PackageWriter();
    void Add( const boost::filesystem::path& path, const boost::filesystem::path& pathInArchive );
    bool Save();
private:
    std::auto_ptr<detail::PackageImpl> mImpl;
};

} // namespace platform

#endif//INCLUDED_PLATFORM_PACKAGE_H
