#ifndef INCLUDED_PLATFORM_TEXTURE_BASE_H
#define INCLUDED_PLATFORM_TEXTURE_BASE_H

#include <vector>
#include <stdint.h>
#include <cstddef>

namespace platform {

class TextureBase
{
public:
    TextureBase();
    virtual ~TextureBase();
    size_t GetWidth()const;
    size_t GetHeight()const;
    size_t GetChannels()const;
    uint8_t const* GetData()const;
    bool IsValid()const;
protected:
    size_t mWidth;
    size_t mHeight;
    static const size_t mChannels;
    std::vector<uint8_t> mData;

    static void ConvertRGBtoRGBA( const unsigned char* rgb, int PixelWidth, unsigned char* rgba );

};

} // namespace platform

#endif//INCLUDED_PLATFORM_TEXTURE_BASE_H
