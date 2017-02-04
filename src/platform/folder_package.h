#ifndef INCLUDED_PLATFORM_FOLDER_PACKAGE_H
#define INCLUDED_PLATFORM_FOLDER_PACKAGE_H

#include "package.h"

namespace platform {

class FolderPackage : public IPackage
{
public:
    explicit FolderPackage( boost::filesystem::path const& root );
    virtual ~FolderPackage();
    virtual std::unique_ptr<File> Open( boost::filesystem::path const& path );
    virtual void GetFileNames( PathVect_t& paths, boost::filesystem::path const& dir = boost::filesystem::path() );
    virtual uint32_t Checksum() const;
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // namespace platform

#endif // INCLUDED_PLATFORM_FOLDER_PACKAGE_H

