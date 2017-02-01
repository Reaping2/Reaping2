#ifndef INCLUDED_PLATFORM_FILESYSTEM_H
#define INCLUDED_PLATFORM_FILESYSTEM_H

#include "singleton.h"

#include <memory>
#include <boost/filesystem/path.hpp>
#include <vector>

namespace platform {
class File;
class IPackage;
namespace detail {
class FilesysImpl;
} // namespace detail

class Filesys : public Singleton<Filesys>
{
    friend class Singleton<Filesys>;
    Filesys();
    std::auto_ptr<detail::FilesysImpl> mImpl;
public:
    ~Filesys();
    void Mount( std::auto_ptr<IPackage> Pack );
    std::auto_ptr<File> Open( const boost::filesystem::path& Path );
    void GetFileNames( std::vector<boost::filesystem::path>& Paths, boost::filesystem::path const& Dir = boost::filesystem::path() );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_FILESYSTEM_H
