#ifndef INCLUDED_PLATFORM_DETAIL_TEXTUREIMPL_H
#define INCLUDED_PLATFORM_DETAIL_TEXTUREIMPL_H

#include <vector>
#include <cstddef>
#include <stdint.h>

namespace platform {
namespace detail {
class TextureImpl
{
public:
    TextureImpl();
    size_t mWidth;
    size_t mHeight;
    static const size_t mChannels;
    std::vector<uint8_t> mData;
    static void ConvertRGBtoRGBA( const unsigned char* rgb, int PixelWidth, unsigned char* rgba );
};


} // namespace detail
} // namespace platform

#endif // INCLUDED_PLATFORM_DETAIL_TEXTUREIMPL_H

