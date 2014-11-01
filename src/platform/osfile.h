#ifndef INCLUDED_PLATFORM_OSFILE_H
#define INCLUDED_PLATFORM_OSFILE_H

#include "ifile.h"
#include <boost/filesystem.hpp>
#include <memory>
#include <iosfwd>

namespace platform {
namespace detail {
class OsFileImpl;
} // namespace detail

class OsFile : public File
{
    std::auto_ptr< detail::OsFileImpl > mImpl;
public:
    OsFile( const boost::filesystem::path& Path, std::ios_base::openmode OpenMode = std::ios_base::in );
    ~OsFile();
    using File::Read;
    virtual bool Read( void* Dst, size_t Size );
    using File::Write;
    virtual bool Write( void const* Src, size_t Size );
    virtual bool IsValid()const;
    virtual size_t GetSize()const;
    virtual size_t GetPosition()const;
    virtual bool SetPosition( size_t Pos );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_OSFILE_H
