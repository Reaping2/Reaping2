#include "i_audio.h"

int AudioPlayer::PlayCallback( const void*, void* OutputBuffer,
                               unsigned long FramesPerBuffer,
                               const PaStreamCallbackTimeInfo*,
                               PaStreamCallbackFlags,
                               void* UserData )
{
    return Get().PlayCallbackImpl( OutputBuffer );
}

void AudioPlayer::Play( int32_t EffectUID, int32_t id, glm::vec2 const& pos )
{
    {
        boost::mutex::scoped_lock g( mReadMtx );
        if( std::end( mNewFiles ) != std::find_if( std::begin( mNewFiles ), std::end( mNewFiles ),
                [&]( AudioFile& f ) { return f.GetUID() == EffectUID && ( f.SetPosition( pos ), true ); } ) ||
            std::end( mActiveFiles ) != std::find_if( std::begin( mActiveFiles ), std::end( mActiveFiles ),
                [&]( AudioFile& f ) { return f.GetUID() == EffectUID && ( f.SetPosition( pos ), true ); } ) )
        {
            return;
        }
    }
    static audio::AudioEffectRepo& aer( audio::AudioEffectRepo::Get() );
    audio::AudioEffect const& ae = aer( id );
    if( ae.Path.empty() )
    {
        return;
    }
    std::auto_ptr<AudioFile> F = AudioFile::Create( EffectUID, ae, pos );
    if( !F.get() )
    {
        return;
    }
    boost::mutex::scoped_lock g( mReadMtx );
    mNewFiles.push_back( F.release() );
}

void AudioPlayer::Halt( int32_t EffectUID )
{
    boost::mutex::scoped_lock g( mReadMtx );
    mHaltedIds.push_back( EffectUID );
}

void AudioPlayer::PlayThread()
{
    do
    {
        PaError err = Pa_Initialize();
        if( err != paNoError )
        {
            break;
        }

        mOutputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        if ( mOutputParameters.device == paNoDevice )
        {
            break;
        }

        mOutputParameters.channelCount = mNumChannels; /* stereo output */
        mOutputParameters.sampleFormat = paFloat32;
        mOutputParameters.suggestedLatency = Pa_GetDeviceInfo( mOutputParameters.device )->defaultLowOutputLatency;
        mOutputParameters.hostApiSpecificStreamInfo = NULL;

        err = Pa_OpenStream(
                  &mStream,
                  NULL,/* no input */
                  &mOutputParameters,
                  mPreferredSampleRate,
                  mFramesPerBuffer,
                  paNoFlag, /* we won't output out of range samples so don't bother clipping them */
                  AudioPlayer::PlayCallback,
                  NULL );
        if( err != paNoError )
        {
            break;
        }
        if( !mStream )
        {
            break;
        }
        err = Pa_StartStream( mStream );
        if( err != paNoError )
        {
            break;
        }

        while( ( err = Pa_IsStreamActive( mStream ) ) == 1 )
        {
            Pa_Sleep( 50 );
        }
        if( err < 0 )
        {
            break;
        }

        err = Pa_CloseStream( mStream );
    }
    while( false );

    Pa_Terminate();
}

bool AudioPlayer::IsSampleRateSupported( int32_t Rate ) const
{
    return std::find( mSupportedSampleRates.begin(), mSupportedSampleRates.end(), Rate ) != mSupportedSampleRates.end();
}

AudioPlayer::AudioPlayer()
    : mPreferredSampleRate( 44100 )
    , mFramesPerBuffer( 64 )
    , mClosing( false )
    , mNumChannels( 2 )
    , mBuffer( mNumChannels )
    , mOutputParameters()
    , mStream( NULL )
    , mPlaybackBuffer( NULL )
    , mPlaybackBufferLength( 0 )
{
    mSupportedSampleRates.push_back( mPreferredSampleRate ); // cd qual only, sry :(
    mPhaseChangeId = EventServer<PhaseChangedEvent>::Get().Subscribe( boost::bind( &AudioPlayer::OnPhaseChangedEvent, this, _1 ) );
    mReadThread = boost::thread( boost::bind( &AudioPlayer::ReadThread, this ) );
    mPlaybackThread = boost::thread( boost::bind( &AudioPlayer::PlayThread, this ) );
}

int AudioPlayer::PlayCallbackImpl( void* OutputBuffer )
{
    size_t Ctr = 0;
    float* WritePointer = ( float* )OutputBuffer;
    do
    {
        boost::mutex::scoped_lock g( mPlayMtx );
        size_t const Avail = std::min<size_t>( mFramesPerBuffer, mBuffer.GetSize() );
        if( !Avail )
        {
            break;
        }
        float const* LeftPtr = mBuffer.GetData( 0 );
        float const* RightPtr = mBuffer.GetData( 1 );
        for( ; Ctr < Avail; ++Ctr )
        {
            *WritePointer++ = *LeftPtr++;
            *WritePointer++ = *RightPtr++;
        }
        mBuffer.Read( Avail );
    }
    while( false );
    for( ; Ctr < mFramesPerBuffer; ++Ctr )
    {
        *WritePointer++ = 0;
        *WritePointer++ = 0;
    }
    return GetStatus();
}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
    if( Evt.CurrentPhase == ProgramPhase::CloseSignal )
    {
        mClosing = true;
        mPlaybackThread.join();
        mReadThread.join();
    }
}

int AudioPlayer::GetStatus()
{
    return mClosing ? paComplete : paContinue;
}

std::vector<int32_t> AudioPlayer::HaltableEffects()
{
    boost::mutex::scoped_lock g( mReadMtx );
    return mRunningIds;
}

void AudioPlayer::ReadThread()
{
    while( GetStatus() == paContinue )
    {
        std::vector<int32_t> haltedIds, runningIds;
        {
            // merge new files
            boost::mutex::scoped_lock g( mReadMtx );
            mActiveFiles.transfer( mActiveFiles.end(), mNewFiles );
            // swap halted files
            std::swap( mHaltedIds, haltedIds );
        }
        static const size_t DefaultCommonSize = 4096;
        size_t MinCommonSize = DefaultCommonSize;
        for( AudioFiles_t::iterator i = mActiveFiles.begin(), e = mActiveFiles.end(); i != e; ++i )
        {
            // preload active files
            AudioFile& f = *i;
            f.FillBufferIfNeeded();
            MinCommonSize = std::min<size_t>( MinCommonSize, f.GetBuffer().GetSize() );
        }
        if( !MinCommonSize || mBuffer.GetSize() >= DefaultCommonSize )
        {
            boost::this_thread::sleep_for( boost::chrono::milliseconds( 50 ) );
            continue;
        }
        {
            boost::mutex::scoped_lock g( mPlayMtx );
            mMixer.Mix( mBuffer, mActiveFiles, MinCommonSize );
        }
        for( AudioFiles_t::iterator i = mActiveFiles.begin(); i != mActiveFiles.end(); )
        {
            AudioFile& f = *i;
            if( f.IsFinishedPlaying() ||
                ( f.IsInterruptable() && std::end( haltedIds ) != std::find( haltedIds.begin(), haltedIds.end(), f.GetUID() ) ) )
            {
                i = mActiveFiles.erase( i );
            }
            else
            {
                if( f.IsInterruptable() )
                {
                    runningIds.push_back( f.GetUID() );
                }
                ++i;
            }
        }
        boost::mutex::scoped_lock g( mReadMtx );
        std::swap( mRunningIds, runningIds );
    }
}
