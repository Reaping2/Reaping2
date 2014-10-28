#include "i_platform.h"

TgaTexture::TgaTexture( File& F )
{
    if( F.IsValid() )
    {
        Load( F );
    }
}

void TgaTexture::Load( File& F )
{
    Header Head;
    if( !F.Read( Head.mData, sizeof( Header ) ) )
    {
        return;
    }
    mWidth = Head.Width();
    mHeight = Head.Height();
    assert( Head.PixelDepth() == 24 || Head.PixelDepth() == 32 );
    if( Head.PixelDepth() != 24 && Head.PixelDepth() != 32 )
    {
        return;
    }
    size_t const ImgSize = mWidth * mHeight;
    size_t const DataSize = mChannels * ImgSize;
    mData.reset( new uint8_t[DataSize] );
    bool Succ;
    if( Head.PixelDepth() == 32 )
    {
        assert( ( Head.ImageDescriptor() & 15 ) == 8 );
        Succ = F.Read( ( void* )mData.get(), DataSize );
    }
    else
    {
        std::string Buf;
        size_t const InSize = ImgSize * 3;
        Succ = F.Read( Buf, InSize );
        if( Succ )
        {
            ConvertRGBtoRGBA( ( uint8_t* )( void* )Buf.c_str(), ImgSize, mData.get() );
        }
    }
    if( !Succ )
    {
        mData.reset();
    }
    else
    {
        Reorder( mData.get(), ImgSize );
    }
}

void TgaTexture::Reorder( uint8_t* Data, size_t Size )
{
    for( size_t x = 0; x < Size; x++ )
    {
        unsigned char* pixel_out = &Data[x * 4];
        std::swap( pixel_out[0], pixel_out[2] );
    }
}
