#ifndef INCLUDED_PLATFORM_MEMORYFILE_H
#define INCLUDED_PLATFORM_MEMORYFILE_H

class MemoryFile : public File
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
    MemoryFile( size_t PreallocatedBuffers = 0 );
    virtual bool Read( void* Dst, size_t Size );
    virtual bool Read( std::string& Data, size_t Size )
    {
        return File::Read( Data, Size );
    }
    virtual bool Write( void const* Src, size_t Size );
    virtual bool Write( std::string const& Data )
    {
        return File::Write( Data );
    }
    virtual size_t GetSize()const;
    virtual size_t GetPosition()const;
    virtual bool IsValid()const;
    virtual bool SetPosition( size_t Pos );

    bool CopyTo( File& Other );
    bool CopyFrom( File& Other );
    bool Rewind(); // talan File is tudhatna
};

#endif//INCLUDED_PLATFORM_MEMORYFILE_H
