#include "texture_base.h"

namespace platform {

TextureBase::~TextureBase()
{
}

size_t TextureBase::GetWidth() const
{
    return mWidth;
}

size_t TextureBase::GetHeight() const
{
    return mHeight;
}

size_t TextureBase::GetChannels() const
{
    return mChannels;
}

uint8_t const* TextureBase::GetData() const
{
    return mData.data();
}

TextureBase::TextureBase()
    : mWidth( 0 )
    , mHeight( 0 )
{

}

bool TextureBase::IsValid() const
{
    return !mData.empty();
}

void TextureBase::ConvertRGBtoRGBA( const unsigned char* rgb, int PixelWidth, unsigned char* rgba )
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

const size_t TextureBase::mChannels = 4;

} // namespace platform

