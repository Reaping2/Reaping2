#ifndef INCLUDED_PLATFORM_SERIALIZE_H
#define INCLUDED_PLATFORM_SERIALIZE_H

#include <string>
#include "rstdint.h"

namespace platform {
// data filejaink little endianok, mindig
namespace conv {
inline uint16_t bswap( uint16_t val )
{
    return ( val << 8 ) | ( val >> 8 );
}
inline int16_t bswap( int16_t val )
{
    return ( val << 8 ) | ( ( val >> 8 ) & 0xFF );
}
inline uint32_t bswap( uint32_t val )
{
    val = ( ( val << 8 ) & 0xFF00FF00 ) | ( ( val >> 8 ) & 0xFF00FF );
    return ( val << 16 ) | ( val >> 16 );
}
inline int32_t bswap( int32_t val )
{
    val = ( ( val << 8 ) & 0xFF00FF00 ) | ( ( val >> 8 ) & 0xFF00FF );
    return ( val << 16 ) | ( ( val >> 16 ) & 0xFFFF );
}

#ifdef BOOST_BIG_ENDIAN
#   define ToLittleEndian(x) bswap(x)
#   define FromLittleEndian(x) bswap(x)
#else
#   define ToLittleEndian(x) (x)
#   define FromLittleEndian(x) (x)
#endif//BOOST_BIG_ENDIAN

template<typename T>
std::string serialize( T t );


template<typename T>
std::string serialize( T t )
{
    std::string r( sizeof( T ), '\0' );
    t = ToLittleEndian( t );
    memcpy( &r[0], &t, sizeof( T ) );
    return r;
}

template<typename T>
bool deserialize( const std::string& str, T& t );

template<typename T>
bool deserialize( const std::string& str, T& t )
{
    if( str.size() != sizeof( T ) )
    {
        return false;
    }
    T tmp;
    memcpy( &tmp, &str[0], sizeof( T ) );
    t = FromLittleEndian( tmp );
    return true;
}

#undef ToLittleEndian
#undef FromLittleEndian

} // namespace conv
} // namespace platform

#endif//INCLUDED_PLATFORM_SERIALIZE_H
