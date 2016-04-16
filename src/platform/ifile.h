#ifndef INCLUDED_PLATFORM_IFILE_H
#define INCLUDED_PLATFORM_IFILE_H

#include <string>

namespace platform {

class File
{
public:
    virtual ~File() = 0;
    virtual bool Read( void* Dst, size_t Size ) = 0;
    virtual bool Read( std::string& Data, size_t Size );
    virtual bool Write( void const* Src, size_t Size ) = 0;
    virtual bool Write( const std::string& Data );
    virtual bool ReadAll( std::string& Data );
    virtual size_t GetSize()const = 0;
    virtual size_t GetPosition()const = 0;
    virtual bool SetPosition( size_t Pos ) = 0;
    virtual bool IsValid()const = 0;
};

} // namespace platform


#endif//INCLUDED_PLATFORM_IFILE_H
