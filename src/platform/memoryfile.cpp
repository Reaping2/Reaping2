#include "memoryfile.h"

#include <vector>
#include <string.h>
#include <boost/assert.hpp>

namespace platform {
namespace detail {

class MemoryFileImpl
{
    enum
    {
        ONE_BUFFER_SIZE = 4096,
    };
    struct Buffer_t
    {
        char data[ONE_BUFFER_SIZE];
        size_t size;
        size_t pos;
        size_t write( void const* d, size_t s );
        size_t read( void* d, size_t s );
        Buffer_t(): size( 0 ), pos( 0 ) {}
    };
    typedef std::vector<Buffer_t> Buffers;
    Buffers mBuffers;
    size_t mActBufferId;
    size_t mSize;
    // sok realloc kene, ha siman osszefuggo teruletre akarnank pakolni
    // nyilvan igy meg a read lassul, de a filerendszeres megoldassal kb szinten van
    // szal a read performance miatt nem parazok
public:
    MemoryFileImpl( size_t PreallocatedBuffers = 0 );
    bool Read( void* Dst, size_t Size );
    bool Write( void const* Src, size_t Size );
    size_t GetSize()const;
    size_t GetPosition()const;
    bool IsValid()const;
    bool SetPosition( size_t Pos );

    bool CopyTo( File& Other );
    bool CopyFrom( File& Other );
    bool Rewind(); // talan File is tudhatna
};


MemoryFileImpl::MemoryFileImpl( size_t PreallocatedBuffers/*=0*/ )
    : mActBufferId( 0 )
    , mSize( 0 )
{
    if( PreallocatedBuffers )
    {
        mBuffers.resize( PreallocatedBuffers );
    }
}

bool MemoryFileImpl::Read( void* Data, size_t Size )
{
    if( Size > GetSize() - GetPosition() )
    {
        return false;
    }
    char* p = ( char* )Data;
    while( Size )
    {
        const size_t R = mBuffers[mActBufferId].read( p, Size );
        Size -= R;
        p += R;
        if( Size )
        {
            mActBufferId++;
        }
        if( mActBufferId >= mBuffers.size() )
        {
            return false;
        }
    }
    return true;
}

size_t MemoryFileImpl::GetSize() const
{
    return mSize;
}

bool MemoryFileImpl::Write( void const* Src, size_t Size )
{
    mSize += Size;
    char const* p = ( char const* )Src;
    while( Size )
    {
        if( mActBufferId >= mBuffers.size() )
        {
            mBuffers.push_back( Buffer_t() );
        }
        const size_t W = mBuffers[mActBufferId].write( p, Size );
        Size -= W;
        p += W;
        if( Size )
        {
            mActBufferId++;
        }
    }
    return true;
}

bool MemoryFileImpl::IsValid() const
{
    return true;
}

bool MemoryFileImpl::CopyTo( File& Other )
{
    for( Buffers::const_iterator i = mBuffers.begin(), e = mBuffers.end(); i != e; ++i )
    {
        if( !i->size )
        {
            break;
        }
        std::string Data( i->data, i->size );
        if( !Other.Write( Data ) )
        {
            return false;
        }
    }
    return true;
}

bool MemoryFileImpl::CopyFrom( File& Other )
{
    size_t Size = Other.GetSize();
    mActBufferId = 0;
    mSize = Size;
    const size_t NumBuffers = Size / ONE_BUFFER_SIZE + ( Size % ONE_BUFFER_SIZE ) ? 1 : 0;
    mBuffers.clear();
    mBuffers.resize( NumBuffers );
    size_t Idx = 0;
    while( Size )
    {
        size_t ToRead = std::min<size_t>( Size, ONE_BUFFER_SIZE );
        std::string Buffer;
        if( !Other.Read( Buffer, ToRead ) )
        {
            mBuffers.clear();
            mSize = 0;
            return false;
        }
        memcpy( mBuffers[Idx++].data, Buffer.data(), ToRead );
        Size -= ToRead;
    }
    return true;
}

bool MemoryFileImpl::Rewind()
{
    mActBufferId = 0;
    for( Buffers::iterator i = mBuffers.begin(), e = mBuffers.end(); i != e; ++i )
    {
        i->pos = 0;
    }
    return true;
}

size_t MemoryFileImpl::GetPosition() const
{
    return mActBufferId * ONE_BUFFER_SIZE + ( ( mActBufferId < mBuffers.size() ) ? mBuffers[mActBufferId].pos : 0 );
}

bool MemoryFileImpl::SetPosition( size_t Pos )
{
    if( Pos == 0 )
    {
        return Rewind();
    }
    assert( false );
    // ha kell, implementald, pillanatnyilag nem latom ertelmet
    return false;
}

size_t MemoryFileImpl::Buffer_t::write( void const* d, size_t s )
{
    const size_t Avail = ONE_BUFFER_SIZE - pos;
    const size_t ToWrite = std::min<size_t>( Avail, s );
    memcpy( data + pos, d, ToWrite );
    pos += ToWrite;
    if( pos > size )
    {
        size = pos;
    }
    return ToWrite;
}

size_t MemoryFileImpl::Buffer_t::read( void* d, size_t s )
{
    const size_t ToRead = std::min<size_t>( size - pos, s );
    memcpy( d, data + pos, ToRead );
    pos += ToRead;
    return ToRead;
}

} // namespace detail

MemoryFile::MemoryFile( size_t PreallocatedBuffers )
{
     mImpl.reset( new detail::MemoryFileImpl( PreallocatedBuffers ) );
}

MemoryFile::~MemoryFile()
{
}

bool MemoryFile::Read( void* Dst, size_t Size )
{
    return mImpl->Read( Dst, Size );
}

bool MemoryFile::Write( void const* Src, size_t Size )
{
    return mImpl->Write( Src, Size );
}

size_t MemoryFile::GetSize() const
{
    return mImpl->GetSize();
}

size_t MemoryFile::GetPosition() const
{
    return mImpl->GetPosition();
}

bool MemoryFile::IsValid() const
{
    return mImpl->IsValid();
}

bool MemoryFile::SetPosition( size_t Pos )
{
    return mImpl->SetPosition( Pos );
}

bool MemoryFile::CopyTo( File& Other )
{
    return mImpl->CopyTo( Other );
}

bool MemoryFile::CopyFrom( File& Other )
{
    return mImpl->CopyFrom( Other );
}

bool MemoryFile::Rewind()
{
    return mImpl->Rewind();
}

} // namespace platform

