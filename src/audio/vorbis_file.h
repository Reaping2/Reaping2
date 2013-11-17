#ifndef INCLUDED_AUDIO_VORBIS_FILE_H
#define INCLUDED_AUDIO_VORBIS_FILE_H

class VorbisFile
{
	static const size_t TargetBufferSize;
	OggVorbis_File mVorbisFile;
	vorbis_info mInfo;
	int mEof;
	int mCurrentSection;
	AutoFile mSource;
	std::auto_ptr<AudioBuffer> mBuffer;
	enum Status {
		St_None,
		St_Inited,
		St_Closed,
	};
	Status mStatus;

	bool Init();
	bool ReadInfo();
	bool ReadBlock();

	static size_t ReadFunc(void* Buffer,size_t Size, size_t Count, void* This);
	static int CloseFunc(void* This);
	VorbisFile(AutoFile File);
public:
	static std::auto_ptr<VorbisFile> Create(AutoFile File);
	~VorbisFile();
	bool FillBufferIfNeeded();
	AudioBuffer& GetBuffer();
	bool IsFinished()const;
};

#endif//INCLUDED_AUDIO_VORBIS_FILE_H
