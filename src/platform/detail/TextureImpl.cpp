#include "TextureImpl.h"

namespace platform {
namespace detail {
TextureImpl::TextureImpl()
    : mWidth( 0 )
    , mHeight( 0 )
{
}

void TextureImpl::ConvertRGBtoRGBA( const unsigned char* rgb, int PixelWidth, unsigned char* rgba )
{
    for( int x = 0; x < PixelWidth; x++ )
    {
        const unsigned char* pixel_in = &rgb[x * 3];
        unsigned char* pixel_out = &rgba[x * 4];
        pixel_out[0] = pixel_in[0];
        pixel_out[1] = pixel_in[1];
        pixel_out[2] = pixel_in[2];
        pixel_out[3] = 0xff;
    }
}

const size_t TextureImpl::mChannels = 4;

} // namespace detail
} // namespace platform

