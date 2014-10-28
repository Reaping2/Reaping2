#ifndef INCLUDED_PLATFORM_TGATEXTURE_H
#define INCLUDED_PLATFORM_TGATEXTURE_H

class TgaTexture : public TextureBase
{
public:
    TgaTexture( File& F );
    static void Reorder( uint8_t* Data, size_t Size );
private:
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
    void Load( File& F );
};

#endif//INCLUDED_PLATFORM_TGATEXTURE_H
