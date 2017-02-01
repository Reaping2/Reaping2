#ifndef INCLUDED_PLATFORM_IPACKAGE_H
#define INCLUDED_PLATFORM_IPACKAGE_H

#include "rstdint.h"
#include <boost/filesystem/path.hpp>
#include <memory>
#include <vector>

namespace platform {
class File;
typedef std::vector<boost::filesystem::path> PathVect_t;

class IPackage
{
public:
    virtual ~IPackage() {};
    virtual std::unique_ptr<File> Open( boost::filesystem::path const& path ) = 0;
    virtual void GetFileNames( PathVect_t& paths, boost::filesystem::path const& dir = boost::filesystem::path() ) = 0;
    virtual uint32_t Checksum() const = 0;
};

} // namespace platform

#endif // INCLUDED_PLATFORM_IPACKAGE_H

