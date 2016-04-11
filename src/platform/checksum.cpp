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
        std::string dataNoEol("");
        size_t eolPos = 0;
        // convert EOL \r\n (win) to \r (UNIX-like including Mac OS) to make checksum checking cross platform
        while( true )
        {
            size_t last = eolPos;
            eolPos = data.find("\r\n", last );
            if ( std::string::npos != eolPos )
            {
                dataNoEol += (data.substr( last, eolPos-last )+'\n');
                eolPos++;
            }
            else
            {
                break;
            }
        }

        boost::crc_32_type result;
        result.process_bytes( dataNoEol.data(), dataNoEol.length() );
        checksum = result.checksum();
        return checksum;
    }

}
