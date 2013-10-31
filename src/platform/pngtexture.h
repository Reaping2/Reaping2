#ifndef INCLUDED_PLATFORM_PNGTEXTURE_H
#define INCLUDED_PLATFORM_PNGTEXTURE_H

class PngTexture
{
public:
	PngTexture(File& F);
	~PngTexture();
	size_t GetWidth()const;
	size_t GetHeight()const;
	size_t GetChannels()const;
	uint8_t const* GetData()const;
	bool IsValid()const;
private:
	class PngLoadData
	{
		PngLoadData();
	public:
		static PngLoadData* Create();
		~PngLoadData();
		png_structp mPngPtr;
		png_infop mInfoPtr;
		bool mNeedConversion;
		bool mFinished;
	};
	std::auto_ptr<PngLoadData> mLoadData;

	size_t mWidth;
	size_t mHeight;
	std::auto_ptr<uint8_t> mData;

	static const size_t mChannels;
	static const double mMaxGamma;
	static const double mDefaultGamma;
	static const double mInverseGamma;

	bool Load(File& F);
	bool ProcessData(png_bytep Buffer, png_uint_32 Length);
	static bool IsSupportedInputChannelNum(size_t Channels);
	static void InfoCallback(png_structp PngPtr, png_infop InfoPtr);
	static void RowCallback(png_structp PngPtr,png_bytep NewRow,png_uint_32 RowNum,int Pass);
	static void EndCallback(png_structp png_ptr, png_infop info);
};

#endif//INCLUDED_PLATFORM_PNGTEXTURE_H
