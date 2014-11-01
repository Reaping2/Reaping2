#include "package.h"

#include "compression.h"
#include "memoryfile.h"
#include "ifile.h"
#include "serialize.h"
#include "osfile.h"
#include <map>
#include <stdint.h>
#include <boost/filesystem.hpp>

namespace platform {
namespace detail {

class PackageImpl
{
    typedef std::map<boost::filesystem::path, boost::filesystem::path> PathMap;
    PathMap mPaths;

    struct Header
    {
        uint32_t Magic;
        uint32_t Version;
        uint32_t Checksum;
        uint32_t NumFiles;
        Header(): Magic( 0x5a474b50 ), Version( 1 ), Checksum( 0 ), NumFiles() {}
    };
    Header mHeader;
    struct FileDesc
    {
        uint32_t Offset;
        uint32_t FileSize;
    };
    typedef std::map<boost::filesystem::path, FileDesc> FilesMap;
    FilesMap mFiles;
    std::auto_ptr<File> mFile;
public:
    PackageImpl( std::auto_ptr<File> F );
    bool LoadHeader();
    bool WriteHeader();
    std::auto_ptr<File> Open( const boost::filesystem::path& Path );
    void GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir = boost::filesystem::path() );
    void Add( const boost::filesystem::path& Path, const boost::filesystem::path& PathInArchive );
    bool Save();
};

bool PackageImpl::LoadHeader()
{
    if( !mFile.get() || !mFile->IsValid() )
    {
        return false;
    }
    uint32_t BufferSize;
    std::string Buffer;
    if( !mFile->Read( Buffer, sizeof( BufferSize ) ) || !conv::deserialize( Buffer, BufferSize ) )
    {
        return false;
    }
    if( !mFile->Read( Buffer, BufferSize ) )
    {
        return false;
    }
    if( !Compression::Get().Inflate( Buffer, Buffer ) )
    {
        return false;
    }
    MemoryFile f;
    if( !f.Write( Buffer ) )
    {
        return false;
    }
    if( !f.Rewind() )
    {
        return false;
    }
    if( !f.Read( Buffer, sizeof( mHeader.Magic ) ) || !conv::deserialize( Buffer, mHeader.Magic ) )
    {
        return false;
    }
    if( !f.Read( Buffer, sizeof( mHeader.Version ) ) || !conv::deserialize( Buffer, mHeader.Version ) )
    {
        return false;
    }
    if( !f.Read( Buffer, sizeof( mHeader.Checksum ) ) || !conv::deserialize( Buffer, mHeader.Checksum ) )
    {
        return false;
    }
    if( !f.Read( Buffer, sizeof( mHeader.NumFiles ) ) || !conv::deserialize( Buffer, mHeader.NumFiles ) )
    {
        return false;
    }
    for( size_t i = 0; i < mHeader.NumFiles; ++i )
    {
        uint32_t FileNameSize = 0;
        if( !f.Read( Buffer, sizeof( FileNameSize ) ) || !conv::deserialize( Buffer, FileNameSize ) )
        {
            return false;
        }
        if( !f.Read( Buffer, FileNameSize ) )
        {
            return false;
        }
        FileDesc& FD = mFiles[boost::filesystem::path( Buffer )];
        if( !f.Read( Buffer, sizeof( FD.Offset ) ) || !conv::deserialize( Buffer, FD.Offset ) )
        {
            return false;
        }
        if( !f.Read( Buffer, sizeof( FD.FileSize ) ) || !conv::deserialize( Buffer, FD.FileSize ) )
        {
            return false;
        }
    }
    const size_t BaseOffset = mFile->GetPosition();
    for( FilesMap::iterator i = mFiles.begin(), e = mFiles.end(); i != e; ++i )
    {
        i->second.Offset += BaseOffset;
    }
    return true;
}

std::auto_ptr<File> PackageImpl::Open( const boost::filesystem::path& Path )
{
    FilesMap::const_iterator it = mFiles.find( Path );
    if( it == mFiles.end() )
    {
        return std::auto_ptr<File>();
    }

    // ez nem igy lesz kesobb, de egyelore ezzel mar lehet dolgozni
    if( !mFile->SetPosition( it->second.Offset ) )
    {
        return std::auto_ptr<File>();
    }
    std::string Buffer;
    if( !mFile->Read( Buffer, it->second.FileSize ) )
    {
        return std::auto_ptr<File>();
    }
    if( !Compression::Get().Inflate( Buffer, Buffer ) )
    {
        return std::auto_ptr<File>();
    }
    MemoryFile* M = new MemoryFile;
    std::auto_ptr<File> F( M );
    if( !M->Write( Buffer ) )
    {
        return std::auto_ptr<File>();
    }
    M->Rewind();
    return F;
}

void PackageImpl::GetFileNames( PathVect_t& Paths, boost::filesystem::path const& Dir )
{
    Paths.reserve( Paths.size() + mFiles.size() );
    std::string const& DirStr = Dir.string();
    for( FilesMap::const_iterator i = mFiles.begin(), e = mFiles.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = i->first;
        if( DirStr.empty() || Path.parent_path().string().find( DirStr ) == 0 )
        {
            Paths.push_back( Path );
        }
    }
}

bool PackageImpl::WriteHeader()
{
    MemoryFile f;
    std::string Buffer;
    mHeader.NumFiles = mFiles.size();
    if( !f.Write( conv::serialize( mHeader.Magic ) ) )
    {
        return false;
    }
    if( !f.Write( conv::serialize( mHeader.Version ) ) )
    {
        return false;
    }
    if( !f.Write( conv::serialize( mHeader.Checksum ) ) )
    {
        return false;
    }
    if( !f.Write( conv::serialize( mHeader.NumFiles ) ) )
    {
        return false;
    }
    for( FilesMap::const_iterator i = mFiles.begin(), e = mFiles.end(); i != e; ++i )
    {
        boost::filesystem::path const& p = i->first;
        const std::string& Path = p.generic_string(); // nem const refet ad vissza, hanem temp-et, tudom. de ez tok valid.
        const FileDesc& Fd = i->second;
        if( !f.Write( conv::serialize( ( uint32_t )Path.size() ) ) )
        {
            return false;
        }
        if( !f.Write( Path ) )
        {
            return false;
        }
        if( !f.Write( conv::serialize( Fd.Offset ) ) )
        {
            return false;
        }
        if( !f.Write( conv::serialize( Fd.FileSize ) ) )
        {
            return false;
        }
    }
    f.Rewind();
    if( !f.ReadAll( Buffer ) )
    {
        return false;
    }
    if( !Compression::Get().Deflate( Buffer, Buffer ) )
    {
        return false;
    }
    return mFile->Write( conv::serialize( ( uint32_t )Buffer.size() ) ) && mFile->Write( Buffer );
}

bool PackageImpl::Save()
{
    if( !mFile.get() || !mFile->IsValid() )
    {
        return false;
    }
    MemoryFile DataParts;
    uint32_t Offset = 0;
    Compression& Comp( Compression::Get() );
    for( PathMap::const_iterator i = mPaths.begin(), e = mPaths.end(); i != e; ++i )
    {
        OsFile In( boost::filesystem::absolute( i->first ) );
        if( !In.IsValid() )
        {
            continue;
        }
        std::string Buffer;
        In.ReadAll( Buffer ); // ez sokszaz megas filenal akar meg fajhat is
        if( !Comp.Deflate( Buffer, Buffer ) )
        {
            continue;
        }
        FileDesc& Desc = mFiles[i->second];
        Desc.FileSize = Buffer.size();
        Desc.Offset = Offset;
        Offset += Desc.FileSize;
        DataParts.Write( Buffer );
    }
    WriteHeader();
    DataParts.CopyTo( *mFile );
    mFile.reset();
    return true;
}

void PackageImpl::Add( const boost::filesystem::path& Path, const boost::filesystem::path& PathInArchive )
{
    mPaths[Path] = PathInArchive;
}

PackageImpl::PackageImpl( std::auto_ptr<File> F )
    : mFile( F )
{

}

} // namespace detail

Package::Package( std::auto_ptr<File> Source )
{
    mImpl.reset( new detail::PackageImpl( Source ) );
    mImpl->LoadHeader();
}

std::auto_ptr<File> Package::Open( boost::filesystem::path const& path )
{
    return mImpl->Open( path );
}

void Package::GetFileNames( PathVect_t& paths, boost::filesystem::path const& dir )
{
    mImpl->GetFileNames( paths, dir );
}

PackageWriter::PackageWriter( std::auto_ptr<File> Target )
{
    mImpl.reset( new detail::PackageImpl( Target ) );
}

void PackageWriter::Add( const boost::filesystem::path& Path, const boost::filesystem::path& PathInArchive )
{
    mImpl->Add( Path, PathInArchive );
}

bool PackageWriter::Save()
{
    return mImpl->Save();
}

} // namespace platform

