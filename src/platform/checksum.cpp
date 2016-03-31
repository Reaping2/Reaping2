#include "checksum.h"
#include "filesystem.h"
#include "ifile.h"

namespace platform {
    boost::uint32_t fileChecksum( std::string const& filename )
    {
        boost::uint32_t checksum(0);
        Filesys& fs( Filesys::Get() );
        std::auto_ptr<File> f( fs.Open( filename ) );
        if ( NULL == f.get() )
        {
            return checksum;
        }
        std::string data;
        f->ReadAll( data );
        boost::crc_32_type result;
        result.process_bytes( data.data(), data.length() );
        checksum = result.checksum();
        return checksum;
    }

}
