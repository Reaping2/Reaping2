#include "i_audio.h"

const size_t AudioFile::TargetBufferSize = 4096;

bool AudioFile::FillBufferIfNeeded()
{
    if( mBuffer.GetSize() >= TargetBufferSize )
    {
        return false;
    }
    size_t Size;
    Size = 2 * TargetBufferSize - mBuffer.GetSize();
    bool SourceFinishedLoading;
    size_t VorbisBufSize;
    {
        boost::mutex::scoped_lock( mSource->GetMutex() );
        SourceFinishedLoading = mSource->IsFinishedLoading();
        bool const ContinueFromStart = ( GetMode() == audio::Repeat ) && SourceFinishedLoading;
        AudioBuffer const& VorbisBuf = mSource->GetBuffer();
        VorbisBufSize = VorbisBuf.GetSize();
        Size = mBuffer.CopyFrom( VorbisBuf, mPosInSource, Size, ContinueFromStart );
    }
    mPosInSource += Size;
    if( !mFinishedPlaying && GetMode() == audio::PlayOnce && SourceFinishedLoading && mPosInSource == VorbisBufSize )
    {
        mFinishedPlaying = true;
    }
    else if( GetMode() == audio::Repeat && SourceFinishedLoading && VorbisBufSize && mPosInSource >= VorbisBufSize )
    {
        mPosInSource = mPosInSource % VorbisBufSize;
    }
    return true;
}

AudioFile::AudioFile( int32_t uid, VorbisFileCache::Elem Source, audio::AudioEffect const& effect, glm::vec2 const& pos )
    : mUID( uid )
    , mEffect( effect )
    , mSource( Source )
    , mPosInSource( 0 )
    , mBuffer( Source->GetBuffer().GetChannels() )
    , mFinishedPlaying( false )
    , mPos( pos )
{

}

glm::vec2 const& AudioFile::GetPosition() const
{
    return mPos;
}

std::auto_ptr<AudioFile> AudioFile::Create( int32_t uid, audio::AudioEffect const& effect, glm::vec2 const& pos )
{
    VorbisFileCache::Elem El = VorbisFileCache::Get().Load( effect.Path );
    return std::auto_ptr<AudioFile>( El.get() ? new AudioFile( uid, El, effect, pos ) : NULL );
}

bool AudioFile::IsFinishedPlaying() const
{
    return mFinishedPlaying && mBuffer.GetSize() == 0;
}

int32_t AudioFile::GetUID() const
{
    return mUID;
}

audio::AudioType AudioFile::GetType() const
{
    return mEffect.Type;
}

audio::PlayMode AudioFile::GetMode() const
{
    return mEffect.Mode;
}

bool AudioFile::IsInterruptable() const
{
    return mEffect.Interruptable;
}

AudioBuffer& AudioFile::GetBuffer()
{
    return mBuffer;
}

void AudioFile::SetPosition( glm::vec2 const& pos )
{
    mPos = pos;
}

double AudioFile::GetVolume() const
{
    return mEffect.Volume;
}

