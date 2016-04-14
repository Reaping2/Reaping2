#include "ifile.h"
#include <boost/crc.hpp>
#include <boost/algorithm/string.hpp>

namespace platform {

File::~File()
{
}

bool File::ReadAll( std::string& Data )
{
    if( !IsValid() )
    {
        return false;
    }
    return Read( Data, GetSize() - GetPosition() );
}

bool File::Read( std::string& Data, size_t Size )
{
    if( !IsValid() )
    {
        return false;
    }
    std::string tmp( Size, '\0' );
    if( !Read( static_cast< void* >( &tmp[0] ), Size ) )
    {
        return false;
    }
    using std::swap;
    swap( Data, tmp );
    return true;
}

bool File::Write( const std::string& Data )
{
    if( !IsValid() )
    {
        return false;
    }
    return Write( static_cast< void const* >( Data.data() ), Data.size() );
}

uint32_t File::Checksum() const
{
    using namespace boost;
    crc_32_type result;
    std::string Data;
    ReadAll(Data);
    //remove EOL
    erase_all(Data,"\n"); 
    erase_all(Data,"\r"); 
    result.process_bytes( Data.data(), Data.length());
    
    return result.checksum();
}

} // namespace platform
