#include "i_platform.h"

namespace
{
	// Converts BGRA->RGBA and RGBA->BGRA.
	void ConvertBetweenBGRAandRGBA(const unsigned char* input, int pixel_width,
		unsigned char* output) {
			for (int x = 0; x < pixel_width; x++) {
				const unsigned char* pixel_in = &input[x * 4];
				unsigned char* pixel_out = &output[x * 4];
				pixel_out[0] = pixel_in[2];
				pixel_out[1] = pixel_in[1];
				pixel_out[2] = pixel_in[0];
				pixel_out[3] = pixel_in[3];
			}
	}
	void ConvertRGBtoRGBA(const unsigned char* rgb, int pixel_width,
		unsigned char* rgba) {
			for (int x = 0; x < pixel_width; x++) {
				const unsigned char* pixel_in = &rgb[x * 3];
				unsigned char* pixel_out = &rgba[x * 4];
				pixel_out[0] = pixel_in[0];
				pixel_out[1] = pixel_in[1];
				pixel_out[2] = pixel_in[2];
				pixel_out[3] = 0xff;
			}
	}

}

PngTexture::PngTexture(File& F)
: mWidth(0)
, mHeight(0)
{
	if(F.IsValid())
		Load(F);
}

bool PngTexture::Load( File& F )
{
	mLoadData.reset(PngLoadData::Create());
	if(!mLoadData.get())return false;

	png_set_progressive_read_fn(mLoadData->mPngPtr,(void*)this,&PngTexture::InfoCallback,&PngTexture::RowCallback,&PngTexture::EndCallback);
	size_t RemainingSize=F.GetSize();
	while(RemainingSize)
	{
		static const size_t MaxPassSize=4096;
		size_t CurrentPass=std::max<size_t>(MaxPassSize,RemainingSize);
		std::string Buffer;
		if(!F.Read(Buffer,CurrentPass))break;
		if(!ProcessData((png_bytep)(void*)&Buffer[0],Buffer.size()))break;
		RemainingSize-=CurrentPass;
	}
	if(RemainingSize||!mLoadData->mFinished)
	{
		mData.reset();
		mWidth=0;
		mHeight=0;
	}
	mLoadData.reset();
	return !!mData.get();
}

bool PngTexture::ProcessData(png_bytep Buffer, png_uint_32 Length)
{
	if (setjmp(png_jmpbuf(mLoadData->mPngPtr)))
		return false;

	png_process_data(mLoadData->mPngPtr,mLoadData->mInfoPtr,Buffer,Length);
	return true;
}

bool PngTexture::IsSupportedInputChannelNum(size_t Channels)
{
	return Channels==3||Channels==4;
}

void PngTexture::InfoCallback(png_structp PngPtr, png_infop InfoPtr)
{
	PngTexture* Self=(PngTexture*)png_get_progressive_ptr(PngPtr);

	int BitDepth, ColorType, InterlaceType, CompressionType;
	int FilterType, Channels;
	png_uint_32 Width, Height;
	png_get_IHDR(PngPtr, InfoPtr, &Width, &Height, &BitDepth, &ColorType,
		&InterlaceType, &CompressionType, &FilterType);

	unsigned long long TotalSize=(unsigned long long)Width*(unsigned long long)Height;
	if(TotalSize > ((1<<29)-1))
		longjmp(png_jmpbuf(PngPtr), 1);

	Self->mWidth=(size_t)Width;
	Self->mHeight=(size_t)Height;

	// Expand to ensure we use 24-bit for RGB and 32-bit for RGBA.
	if (ColorType == PNG_COLOR_TYPE_PALETTE ||
		(ColorType == PNG_COLOR_TYPE_GRAY && BitDepth < 8))
		png_set_expand(PngPtr);

	// Transparency for paletted images.
	if (png_get_valid(PngPtr, InfoPtr, PNG_INFO_tRNS))
		png_set_expand(PngPtr);

	// Convert 16-bit to 8-bit.
	if (BitDepth == 16)
		png_set_strip_16(PngPtr);

	// Expand grayscale to RGB.
	if (ColorType == PNG_COLOR_TYPE_GRAY ||
		ColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(PngPtr);

	// Deal with gamma and keep it under our control.
	double Gamma;
	if (png_get_gAMA(PngPtr, InfoPtr, &Gamma))
	{
		if(Gamma<=0.0 || Gamma>mMaxGamma)
		{
			Gamma=mInverseGamma;
			png_set_gAMA(PngPtr, InfoPtr, Gamma);
		}
		png_set_gamma(PngPtr, mDefaultGamma, Gamma);
	}
	else
		png_set_gamma(PngPtr, mDefaultGamma, mInverseGamma);

	// Tell libpng to send us rows for interlaced pngs.
	if (InterlaceType == PNG_INTERLACE_ADAM7)
		png_set_interlace_handling(PngPtr);

	// Update our info now
	png_read_update_info(PngPtr, InfoPtr);
	Channels = png_get_channels(PngPtr, InfoPtr);

	// Pick our row format converter necessary for this data.
	if(!IsSupportedInputChannelNum(Channels))
		longjmp(png_jmpbuf(PngPtr), 1);
	Self->mLoadData->mNeedConversion=(Channels!=mChannels);
	Self->mData.reset(new uint8_t[Self->mWidth*Self->mHeight*mChannels]);
}

void PngTexture::RowCallback(png_structp PngPtr,png_bytep NewRow,png_uint_32 RowNum,int Pass)
{
	PngTexture* Self=(PngTexture*)png_get_progressive_ptr(PngPtr);
	assert(Pass == 0);
	if(RowNum > Self->mHeight)
	{
		assert(false);
		return;
	}

	uint8_t* Dst=&((Self->mData.get())[Self->mWidth*mChannels*RowNum]);
	if(Self->mLoadData->mNeedConversion)
		ConvertRGBtoRGBA(NewRow,Self->mWidth,Dst);
	else
		memcpy(Dst,NewRow,Self->mWidth*mChannels);
}

void PngTexture::EndCallback(png_structp png_ptr, png_infop info)
{
	PngTexture* Self=(PngTexture*)png_get_progressive_ptr(png_ptr);
	Self->mLoadData->mFinished=true;
}

bool PngTexture::IsValid() const
{
	return !!mData.get();
}

uint8_t const* PngTexture::GetData() const
{
	return mData.get();
}

size_t PngTexture::GetWidth() const
{
	return mWidth;
}

size_t PngTexture::GetHeight() const
{
	return mHeight;
}

size_t PngTexture::GetChannels() const
{
	return mChannels;
}

PngTexture::~PngTexture()
{

}

const double PngTexture::mDefaultGamma=2.2;
const double PngTexture::mInverseGamma=1.0/2.2;
const double PngTexture::mMaxGamma=21474.83;  // Maximum gamma accepted by png library.
const size_t PngTexture::mChannels=4;

PngTexture::PngLoadData::PngLoadData()
: mPngPtr(NULL)
, mInfoPtr(NULL)
, mFinished(false)
, mNeedConversion(false)
{

}

PngTexture::PngLoadData::~PngLoadData()
{
	png_destroy_read_struct(&mPngPtr,&mInfoPtr,(png_infopp)NULL);
}

PngTexture::PngLoadData* PngTexture::PngLoadData::Create()
{
	std::auto_ptr<PngLoadData> LoadData(new PngLoadData);
	LoadData->mPngPtr=png_create_read_struct(PNG_LIBPNG_VER_STRING,(png_voidp)NULL,NULL,NULL);
	if(!LoadData->mPngPtr)return NULL;
	LoadData->mInfoPtr=png_create_info_struct(LoadData->mPngPtr);
	if(!LoadData->mInfoPtr)return NULL;
	if(setjmp(png_jmpbuf(LoadData->mPngPtr)))return NULL;
	return LoadData.release();
}
