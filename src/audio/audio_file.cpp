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
        bool const ContinueFromStart = ( mMode == audio::Repeat ) && SourceFinishedLoading;
        AudioBuffer const& VorbisBuf = mSource->GetBuffer();
        VorbisBufSize = VorbisBuf.GetSize();
        Size = mBuffer.CopyFrom( VorbisBuf, mPosInSource, Size, ContinueFromStart );
    }
    mPosInSource += Size;
    if( !mFinishedPlaying && mMode == audio::PlayOnce && SourceFinishedLoading && mPosInSource == VorbisBufSize )
    {
        mFinishedPlaying = true;
    }
    else if( mMode == audio::Repeat && SourceFinishedLoading && VorbisBufSize && mPosInSource >= VorbisBufSize )
    {
        mPosInSource = mPosInSource % VorbisBufSize;
    }
    return true;
}

AudioFile::AudioFile( VorbisFileCache::Elem Source, audio::PlayMode Mode, audio::AudioType Type, glm::vec2 const& pos )
    : mSource( Source )
    , mPosInSource( 0 )
    , mBuffer( Source->GetBuffer().GetChannels() )
    , mMode( Mode )
    , mType( Type )
    , mFinishedPlaying( false )
    , mPos( pos )
{

}

glm::vec2 const& AudioFile::GetPosition() const
{
    return mPos;
}

std::auto_ptr<AudioFile> AudioFile::Create( boost::filesystem::path const& Path, audio::PlayMode Mode, audio::AudioType Type, glm::vec2 const& pos )
{
    VorbisFileCache::Elem El = VorbisFileCache::Get().Load( Path );
    return std::auto_ptr<AudioFile>( El.get() ? new AudioFile( El, Mode, Type, pos ) : NULL );
}

bool AudioFile::IsFinishedPlaying() const
{
    return mFinishedPlaying && mBuffer.GetSize() == 0;
}

audio::AudioType AudioFile::GetType() const
{
    return mType;
}

AudioBuffer& AudioFile::GetBuffer()
{
    return mBuffer;
}
