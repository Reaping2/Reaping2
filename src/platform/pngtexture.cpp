#include "pngtexture.h"
#include "detail/TextureImpl.h"
#include "ifile.h"

#include <zlib.h>
#include <png.h>
#include <boost/assert.hpp>
#include <cstring>
#include <algorithm>

namespace platform {
namespace {
const double DefaultGamma = 2.2;
const double InverseGamma = 1.0 / 2.2;
const double MaxGamma = 21474.83; // Maximum gamma accepted by png library.


class PngLoadData
{
    PngLoadData( );
    bool Load( File& F );
    png_structp mPngPtr;
    png_infop mInfoPtr;
    bool mNeedConversion;
    bool mFinished;
    std::auto_ptr< detail::TextureImpl > mTexture;
    ~PngLoadData();
public:
    static detail::TextureImpl* Create( File& F );
private:
    bool ProcessData( png_bytep Buffer, png_uint_32 Length );
    static bool IsSupportedInputChannelNum( size_t Channels );
    static void InfoCallback( png_structp PngPtr, png_infop InfoPtr );
    static void RowCallback( png_structp PngPtr, png_bytep NewRow, png_uint_32 RowNum, int Pass );
    static void EndCallback( png_structp PngPtr, png_infop InfoPtr );
};

bool PngLoadData::Load( File& F )
{
    png_set_progressive_read_fn( mPngPtr, ( void* )this, &PngLoadData::InfoCallback, &PngLoadData::RowCallback, &PngLoadData::EndCallback );
    size_t RemainingSize = F.GetSize();
    while( RemainingSize )
    {
        static const size_t MaxPassSize = 4096;
        size_t CurrentPass = std::min<size_t>( MaxPassSize, RemainingSize );
        std::string Buffer;
        if( !F.Read( Buffer, CurrentPass ) )
        {
            break;
        }
        if( !ProcessData( ( png_bytep )( void* )&Buffer[0], Buffer.size() ) )
        {
            break;
        }
        RemainingSize -= CurrentPass;
    }
    if( RemainingSize || !mFinished )
    {
        mTexture->mData.clear();
        mTexture->mWidth = 0;
        mTexture->mHeight = 0;
    }
    return !mTexture->mData.empty();
}

bool PngLoadData::ProcessData( png_bytep Buffer, png_uint_32 Length )
{
    if( setjmp( png_jmpbuf( mPngPtr ) ) )
    {
        return false;
    }

    png_process_data( mPngPtr, mInfoPtr, Buffer, Length );
    return true;
}

bool PngLoadData::IsSupportedInputChannelNum( size_t Channels )
{
    return Channels == 3 || Channels == 4;
}

void PngLoadData::InfoCallback( png_structp PngPtr, png_infop InfoPtr )
{
    PngLoadData* Self = ( PngLoadData* )png_get_progressive_ptr( PngPtr );

    int BitDepth, ColorType, InterlaceType, CompressionType;
    int FilterType, Channels;
    png_uint_32 Width, Height;
    png_get_IHDR( PngPtr, InfoPtr, &Width, &Height, &BitDepth, &ColorType,
                  &InterlaceType, &CompressionType, &FilterType );

    unsigned long long TotalSize = ( unsigned long long )Width * ( unsigned long long )Height;
    if( TotalSize > ( ( 1 << 29 ) - 1 ) )
    {
        longjmp( png_jmpbuf( PngPtr ), 1 );
    }

    Self->mTexture->mWidth = ( size_t )Width;
    Self->mTexture->mHeight = ( size_t )Height;

    // Expand to ensure we use 24-bit for RGB and 32-bit for RGBA.
    if( ColorType == PNG_COLOR_TYPE_PALETTE ||
        ( ColorType == PNG_COLOR_TYPE_GRAY && BitDepth < 8 ) )
    {
        png_set_expand( PngPtr );
    }

    // Transparency for paletted images.
    if( png_get_valid( PngPtr, InfoPtr, PNG_INFO_tRNS ) )
    {
        png_set_expand( PngPtr );
    }

    // Convert 16-bit to 8-bit.
    if( BitDepth == 16 )
    {
        png_set_strip_16( PngPtr );
    }

    // Expand grayscale to RGB.
    if( ColorType == PNG_COLOR_TYPE_GRAY ||
        ColorType == PNG_COLOR_TYPE_GRAY_ALPHA )
    {
        png_set_gray_to_rgb( PngPtr );
    }

    // Deal with gamma and keep it under our control.
    double Gamma;
    if( png_get_gAMA( PngPtr, InfoPtr, &Gamma ) )
    {
        if( Gamma <= 0.0 || Gamma > MaxGamma )
        {
            Gamma = InverseGamma;
            png_set_gAMA( PngPtr, InfoPtr, Gamma );
        }
        png_set_gamma( PngPtr, DefaultGamma, Gamma );
    }
    else
    {
        png_set_gamma( PngPtr, DefaultGamma, InverseGamma );
    }

    // Tell libpng to send us rows for interlaced pngs.
    if ( InterlaceType == PNG_INTERLACE_ADAM7 )
    {
        png_set_interlace_handling( PngPtr );
    }

    // Update our info now
    png_read_update_info( PngPtr, InfoPtr );
    Channels = png_get_channels( PngPtr, InfoPtr );

    // Pick our row format converter necessary for this data.
    if( !IsSupportedInputChannelNum( Channels ) )
    {
        longjmp( png_jmpbuf( PngPtr ), 1 );
    }
    Self->mNeedConversion = ( Channels != detail::TextureImpl::mChannels );
    Self->mTexture->mData.resize( Self->mTexture->mWidth * Self->mTexture->mHeight * detail::TextureImpl::mChannels );
}

void PngLoadData::RowCallback( png_structp PngPtr, png_bytep NewRow, png_uint_32 RowNum, int Pass )
{
    PngLoadData* Self = ( PngLoadData* )png_get_progressive_ptr( PngPtr );
    BOOST_ASSERT( Pass == 0 );
    if( RowNum > Self->mTexture->mHeight )
    {
        BOOST_ASSERT( false );
        return;
    }

    uint8_t* Dst = &( ( &Self->mTexture->mData.at( 0 ) )[Self->mTexture->mWidth * detail::TextureImpl::mChannels * RowNum] );
    if( Self->mNeedConversion )
    {
        detail::TextureImpl::ConvertRGBtoRGBA( NewRow, Self->mTexture->mWidth, Dst );
    }
    else
    {
        memcpy( Dst, NewRow, Self->mTexture->mWidth * detail::TextureImpl::mChannels );
    }
}

void PngLoadData::EndCallback( png_structp PngPtr, png_infop InfoPtr )
{
    PngLoadData* Self = ( PngLoadData* )png_get_progressive_ptr( PngPtr );
    Self->mFinished = true;
}

PngLoadData::PngLoadData()
    : mPngPtr( NULL )
    , mInfoPtr( NULL )
    , mNeedConversion( false )
    , mFinished( false )
{
    mTexture.reset( new detail::TextureImpl );
}

PngLoadData::~PngLoadData()
{
    png_destroy_read_struct( &mPngPtr, &mInfoPtr, ( png_infopp )NULL );
}

detail::TextureImpl* PngLoadData::Create( File& F )
{
    PngLoadData LoadData;
    LoadData.mPngPtr = png_create_read_struct( PNG_LIBPNG_VER_STRING, ( png_voidp )NULL, NULL, NULL );
    if( !LoadData.mPngPtr )
    {
        return NULL;
    }
    LoadData.mInfoPtr = png_create_info_struct( LoadData.mPngPtr );
    if( !LoadData.mInfoPtr )
    {
        return NULL;
    }
    if( setjmp( png_jmpbuf( LoadData.mPngPtr ) ) )
    {
        return NULL;
    }
    if( !LoadData.Load( F ) )
    {
        return NULL;
    }
    return LoadData.mTexture.release();
}

} // namespace anonymous

PngTexture::PngTexture( File& F )
{
    if( F.IsValid() )
    {
        this->mImpl.reset( PngLoadData::Create( F ) );
    }
}


} // namespace platform
