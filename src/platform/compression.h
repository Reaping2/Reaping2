#ifndef INCLUDED_PLATFORM_COMPRESSION_H
#define INCLUDED_PLATFORM_COMPRESSION_H

#include "singleton.h"
#include <string>
#include <zlib.h>

namespace platform {

class Compression : public Singleton<Compression>
{
    friend class Singleton<Compression>;
    Compression();
public:
    ~Compression();
    bool Deflate( std::string& Dest, const std::string& Source, int Level = Z_DEFAULT_COMPRESSION );
    bool Inflate( std::string& Dest, const std::string& Source );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_COMPRESSION_H
