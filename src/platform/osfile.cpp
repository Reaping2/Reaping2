#include "osfile.h"

#include <fstream>

namespace platform {
namespace detail {
class OsFileImpl
{
    std::fstream mFile;
    size_t mSize;
    size_t mPosition;
public:
    OsFileImpl( const boost::filesystem::path& Path, std::ios_base::openmode OpenMode = std::ios_base::in );
    ~OsFileImpl();
    bool Read( void* Dst, size_t Size );
    bool Write( void const* Src, size_t Size );
    bool IsValid()const;
    size_t GetSize()const;
    size_t GetPosition()const;
    bool SetPosition( size_t Pos );
};

OsFileImpl::OsFileImpl( const boost::filesystem::path& Path, std::ios_base::openmode Mode )
    : mFile( Path.c_str(), Mode | std::ios_base::binary )
    , mPosition( 0 )
{
    if( IsValid() && ( Mode & std::ios_base::in ) )
    {
        mFile.seekg( 0, std::ios_base::end );
        mSize = mFile.tellg();
        mFile.seekg( 0, std::ios_base::beg );
    }
    else
    {
        mSize = 0;
    }
}

OsFileImpl::~OsFileImpl()
{
    mFile.close();
}

bool OsFileImpl::IsValid() const
{
    return mFile.good();
}

bool OsFileImpl::Read( void* Data, size_t Size )
{
    if( !IsValid() )
    {
        return false;
    }
    mFile.read( ( char* )Data, Size );
    mPosition = mFile.tellg();
    return IsValid();
}

bool OsFileImpl::Write( void const* Src, size_t Size )
{
    if( !IsValid() )
    {
        return false;
    }
    mSize += Size;
    mFile.write( ( char const* )Src, Size );
    mPosition = mFile.tellg();
    return IsValid();
}

size_t OsFileImpl::GetSize() const
{
    return mSize;
}

size_t OsFileImpl::GetPosition() const
{
    // tellg() is not const, go figure
    return mPosition;
}

bool OsFileImpl::SetPosition( size_t Pos )
{
    mFile.seekg( Pos, std::ios_base::beg );
    mPosition = Pos;
    return size_t( mFile.tellg() ) == Pos;
}

} // namespace detail

OsFile::OsFile( const boost::filesystem::path& Path, std::ios_base::openmode OpenMode )
{
     mImpl.reset( new detail::OsFileImpl( Path, OpenMode ) );
}

OsFile::~OsFile()
{
}

bool OsFile::Read( void* Dst, size_t Size )
{
    return mImpl->Read( Dst, Size );
}

bool OsFile::Write( void const* Src, size_t Size )
{
    return mImpl->Write( Src, Size );
}

size_t OsFile::GetSize() const
{
    return mImpl->GetSize();
}

size_t OsFile::GetPosition() const
{
    return mImpl->GetPosition();
}

bool OsFile::IsValid() const
{
    return mImpl->IsValid();
}

bool OsFile::SetPosition( size_t Pos )
{
    return mImpl->SetPosition( Pos );
}


} // namespace platform

