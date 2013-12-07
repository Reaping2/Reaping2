#ifndef INCLUDED_PLATFORM_TGATEXTURE_H
#define INCLUDED_PLATFORM_TGATEXTURE_H

class TgaTexture : public TextureBase
{
public:
	TgaTexture(File& F);
private:
	struct Header{
		// don't wanna pragma pack
// 		uint8_t IdLength;
// 		uint8_t ColorMapType;
// 		uint8_t ImageType;
// 		uint16_t ColMapStart;
// 		uint16_t ColMapLength;
// 		uint8_t ColMapDepth;
		uint8_t Unused[8];
		uint16_t OriginX;
		uint16_t OriginY;
		uint16_t Width;
		uint16_t Height;
		uint8_t PixelDepth;
		uint8_t ImageDescriptor;
	};
	void Load(File& F);
};

#endif//INCLUDED_PLATFORM_TGATEXTURE_H
