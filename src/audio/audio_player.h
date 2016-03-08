#ifndef INCLUDED_AUDIO_AUDIO_PLAYER_H
#define INCLUDED_AUDIO_AUDIO_PLAYER_H

class AudioPlayer : public Singleton<AudioPlayer>
{
    AudioFiles_t mActiveFiles;
    AudioFiles_t mNewFiles;
    std::vector<int32_t> mHaltedIds;
    std::vector<int32_t> mRunningIds;

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
    AutoReg mPhaseChangeId;
    boost::mutex mReadMtx;
    boost::mutex mPlayMtx;
    boost::atomic<bool> mClosing;
    Mixer mMixer;

    friend class Singleton<AudioPlayer>;
    AudioPlayer();
    void PlayThread();
    void ReadThread();
    int PlayCallbackImpl( void* OutputBuffer );
    int GetStatus();

    static int PlayCallback( const void* InputBuffer, void* OutputBuffer, unsigned long FramesPerBuffer, const PaStreamCallbackTimeInfo* TimeInfo, PaStreamCallbackFlags StatusFlags, void* UserData );
    void OnPhaseChangedEvent( PhaseChangedEvent const& Evt );
public:
    ~AudioPlayer();
    void Play( int32_t EffectUID, int32_t Id, glm::vec2 const& Pos );
    void Halt( int32_t EffectUID );
    std::vector<int32_t> HaltableEffects();
    bool IsSampleRateSupported( int32_t Rate )const;
};

#endif//INCLUDED_AUDIO_AUDIO_PLAYER_H
