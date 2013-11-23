#ifndef INCLUDED_AUDIO_AUDIO_FILE_H
#define INCLUDED_AUDIO_AUDIO_FILE_H

class AudioFile
{
public:
	enum PlayMode
	{
		PlayOnce,
		Repeat,
	};
	enum AudioType
	{
		Music=0,
		Effect,
		Speech,
		NumTypes,
	};
private:
	static const size_t TargetBufferSize;
	VorbisFileCache::Elem mSource;
	size_t mPosInSource;
	AudioBuffer mBuffer;
	PlayMode const mMode;
	AudioType const mType;
	bool mFinishedPlaying;
	AudioFile(VorbisFileCache::Elem Source,PlayMode Mode, AudioType Type);
public:
	bool FillBufferIfNeeded();
	AudioBuffer& GetBuffer();
	AudioType GetType()const;
	bool IsFinishedPlaying()const;
	static std::auto_ptr<AudioFile> Create(boost::filesystem::path const& Path, PlayMode Mode, AudioType Type);
};

typedef boost::ptr_vector<AudioFile> AudioFiles_t;

#endif//INCLUDED_AUDIO_AUDIO_FILE_H
