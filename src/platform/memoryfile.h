#ifndef INCLUDED_PLATFORM_MEMORYFILE_H
#define INCLUDED_PLATFORM_MEMORYFILE_H

#include "ifile.h"
#include <memory>

namespace platform {
namespace detail {
class MemoryFileImpl;
} // namespace detail

class MemoryFile : public File
{
    std::auto_ptr<detail::MemoryFileImpl> mImpl;
public:
    MemoryFile( size_t PreallocatedBuffers = 0 );
    ~MemoryFile();
    using File::Read;
    virtual bool Read( void* Dst, size_t Size );
    using File::Write;
    virtual bool Write( void const* Src, size_t Size );
    virtual size_t GetSize()const;
    virtual size_t GetPosition()const;
    virtual bool IsValid()const;
    virtual bool SetPosition( size_t Pos );

    bool CopyTo( File& Other );
    bool CopyFrom( File& Other );
    bool Rewind(); // talan File is tudhatna
};

} // namespace platform

#endif//INCLUDED_PLATFORM_MEMORYFILE_H
