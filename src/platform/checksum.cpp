#include "checksum.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>

namespace platform {
    // calculates checksum on the given file, to make it multiplatform EOL characters are ripped before checksum
    boost::uint32_t fileChecksum( std::string const& filename )
    {
        boost::uint32_t cs(0);
        std::ifstream ifs( filename );
        if ( !ifs.good() )
        {
            return cs;
        }
        std::stringstream ss;
        ss << ifs.rdbuf();
        std::string data = ss.str();
        ifs.close();

        // remove EOL (\r\n, \n, \r)
        boost::erase_all( data,"\n" );
        boost::erase_all( data,"\r" );

        boost::crc_32_type result;
        result.process_bytes( data.data(), data.length() );
        cs = result.checksum();
        return cs;
    }
}
