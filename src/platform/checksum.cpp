#include "checksum.h"
#include "filesystem.h"
#include "ifile.h"
#include <fstream>

namespace platform {
    boost::uint32_t fileChecksum( std::string const& filename )
    {
        boost::uint32_t checksum(0);
        std::string data;
        // case 1: is it in data.pkg?
        Filesys& fs( Filesys::Get() );
        std::auto_ptr<File> f( fs.Open( filename ) );
        if ( NULL == f.get() )
        {
            // case 2: is it a regular file?
            std::ifstream ifs( filename );
            if ( !ifs.good() )
            {
                return checksum;
            }
            std::stringstream ss;
            ss << ifs.rdbuf();
            data = ss.str();
            ifs.close();
        }
        else
        {
            f->ReadAll( data );
        }
        boost::crc_32_type result;
        result.process_bytes( data.data(), data.length() );
        checksum = result.checksum();
        return checksum;
    }

}
