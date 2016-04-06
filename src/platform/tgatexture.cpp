#include "tgatexture.h"
#include "detail/TextureImpl.h"
#include "ifile.h"
#include <boost/assert.hpp>
#include <algorithm>

namespace platform {
namespace {
struct Header
{
    uint8_t mData[18];
    // don't wanna pragma pack
    uint8_t IdLength()const
    {
        return mData[0];
    }
    uint8_t ColorMapType()const
    {
        return mData[1];
    }
    uint8_t ImageType()const
    {
        return mData[2];
    }
    uint16_t ColMapStart()const
    {
        return mData[3] | ( ( uint16_t )mData[4] << 8 );
    }
    uint16_t ColMapLength()const
    {
        return mData[5] | ( ( uint16_t )mData[6] << 8 );
    }
    uint8_t ColMapDepth()const
    {
        return mData[7];
    }
    uint16_t Origin()const
    {
        return mData[8] | ( ( uint16_t )mData[9] << 8 );
    }
    uint16_t OriginY()const
    {
        return mData[10] | ( ( uint16_t )mData[11] << 8 );
    }
    uint16_t Width()const
    {
        return mData[12] | ( ( uint16_t )mData[13] << 8 );
    }
    uint16_t Height()const
    {
        return mData[14] | ( ( uint16_t )mData[15] << 8 );
    }
    uint8_t PixelDepth()const
    {
        return mData[16];
    }
    uint8_t ImageDescriptor()const
    {
        return mData[17];
    }
};

void Reorder( uint8_t* Data, size_t Size );

detail::TextureImpl* Load( File& F )
{
    Header Head;
    std::auto_ptr<detail::TextureImpl> impl( new detail::TextureImpl );
    if( !F.Read( Head.mData, sizeof( Header ) ) )
    {
        return NULL;
    }
    static uint16_t const MaxSupportedImageSize = 1 << 15;
    assert( Head.Width() <= MaxSupportedImageSize && Head.Height() < MaxSupportedImageSize );
    impl->mWidth = Head.Width();
    impl->mHeight = Head.Height();
    assert( Head.PixelDepth() == 24 || Head.PixelDepth() == 32 );
    if( Head.PixelDepth() != 24 && Head.PixelDepth() != 32 )
    {
        return NULL;
    }
    size_t const ImgSize = impl->mWidth * impl->mHeight;
    size_t const DataSize = detail::TextureImpl::mChannels * ImgSize;
    impl->mData.resize( DataSize );
    bool Succ;
    if( Head.PixelDepth() == 32 )
    {
        assert( ( Head.ImageDescriptor() & 15 ) == 8 );
        Succ = F.Read( static_cast< void* >( &impl->mData.at( 0 ) ), DataSize );
    }
    else
    {
        std::string Buf;
        size_t const InSize = ImgSize * 3;
        Succ = F.Read( Buf, InSize );
        if( Succ )
        {
            detail::TextureImpl::ConvertRGBtoRGBA( ( uint8_t* )( void* )Buf.c_str(), ImgSize, &impl->mData.at( 0 ) );
        }
    }
    if( !Succ )
    {
        impl->mData.clear();
        return NULL;
    }
    else
    {
        Reorder( &impl->mData.at( 0 ), ImgSize );
    }
    return impl.release();
}

void Reorder( uint8_t* Data, size_t Size )
{
    for( size_t x = 0; x < Size; x++ )
    {
        unsigned char* pixel_out = &Data[x * 4];
        std::swap( pixel_out[0], pixel_out[2] );
    }
}
} // namespace anonymous

TgaTexture::TgaTexture( File& F )
{
    if( F.IsValid() )
    {
        mImpl.reset( Load( F ) );
    }
}


} // namespace platform

