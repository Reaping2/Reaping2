#pragma once
#ifndef INCLUDED_CHECKSUM_H
#define INCLUDED_CHECKSUM_H

#include <string>
#include <boost/crc.hpp>

namespace platform {
    boost::uint32_t fileChecksum( std::string const& filename );
}

#endif
