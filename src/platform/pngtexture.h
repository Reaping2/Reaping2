#ifndef INCLUDED_PLATFORM_PNGTEXTURE_H
#define INCLUDED_PLATFORM_PNGTEXTURE_H

class PngTexture : public TextureBase
{
public:
	PngTexture(File& F);
	~PngTexture();
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

	static const double mMaxGamma;
	static const double mDefaultGamma;
	static const double mInverseGamma;

	bool Load(File& F);
	bool ProcessData(png_bytep Buffer, png_uint_32 Length);
	static bool IsSupportedInputChannelNum(size_t Channels);
	static void InfoCallback(png_structp PngPtr, png_infop InfoPtr);
	static void RowCallback(png_structp PngPtr,png_bytep NewRow,png_uint_32 RowNum,int Pass);
	static void EndCallback(png_structp PngPtr, png_infop InfoPtr);
};

#endif//INCLUDED_PLATFORM_PNGTEXTURE_H
