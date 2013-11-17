#ifndef INCLUDED_AUDIO_AUDIO_PLAYER_H
#define INCLUDED_AUDIO_AUDIO_PLAYER_H

class AudioPlayer : public Singleton<AudioPlayer>
{
	typedef boost::ptr_vector<VorbisFile> AudioFiles_t;
	AudioFiles_t mActiveFiles;
	AudioFiles_t mNewFiles;

	static const size_t MinPlaybackBufferSize;
	const size_t mNumChannels;
	AudioBuffer mBuffer;
	int32_t mPreferredSampleRate;
	std::vector<int32_t> mSupportedSampleRates;
	PaStreamParameters mOutputParameters;
	PaStream* mStream;
	unsigned long mFramesPerBuffer;
	float* mPlaybackBuffer;
	size_t mPlaybackBufferLength;
	boost::thread mPlaybackThread;
	boost::thread mReadThread;
	Registration mPhaseChangeId;
	boost::mutex mCloseMtx;
	boost::mutex mReadMtx;
	boost::mutex mPlayMtx;
	bool mClosing;

	friend class Singleton<AudioPlayer>;
	AudioPlayer();
	void PlayThread();
	void ReadThread();
	int PlayCallbackImpl(void *OutputBuffer);
	int GetStatus();

	static int PlayCallback(const void *InputBuffer, void *OutputBuffer, unsigned long FramesPerBuffer, const PaStreamCallbackTimeInfo* TimeInfo, PaStreamCallbackFlags StatusFlags, void *UserData);
	void OnPhaseChangedEvent(PhaseChangedEvent const& Evt);
public:
	~AudioPlayer();
	void Play(boost::filesystem::path const& Path);

	bool IsSampleRateSupported(int32_t Rate)const;
};

#endif//INCLUDED_AUDIO_AUDIO_PLAYER_H
