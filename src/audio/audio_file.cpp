#include "i_audio.h"

const size_t AudioFile::TargetBufferSize=4096;

bool AudioFile::FillBufferIfNeeded()
{
	if(mBuffer.GetSize()>=TargetBufferSize)
		return false;
	size_t Size;
	Size=2*TargetBufferSize-mBuffer.GetSize();
	bool SourceFinishedLoading;
	size_t VorbisBufSize;
	{
		boost::mutex::scoped_lock(mSource->GetMutex());
		SourceFinishedLoading=mSource->IsFinishedLoading();
		bool const ContinueFromStart=(mMode==Repeat)&&SourceFinishedLoading;
		AudioBuffer const& VorbisBuf=mSource->GetBuffer();
		VorbisBufSize=VorbisBuf.GetSize();
		Size=mBuffer.CopyFrom(VorbisBuf,mPosInSource,Size,ContinueFromStart);
	}
	mPosInSource+=Size;
	if(!mFinishedPlaying&&mMode==PlayOnce&&SourceFinishedLoading&&mPosInSource==VorbisBufSize)
		mFinishedPlaying=true;
	else if(mMode==Repeat&&SourceFinishedLoading&&VorbisBufSize&&mPosInSource>=VorbisBufSize)
		mPosInSource=mPosInSource%VorbisBufSize;
	return true;
}

AudioFile::AudioFile( VorbisFileCache::Elem Source,PlayMode Mode, AudioType Type)
:mSource(Source)
,mPosInSource(0)
,mBuffer(Source->GetBuffer().GetChannels())
,mMode(Mode)
,mType(Type)
,mFinishedPlaying(false)
{

}

std::auto_ptr<AudioFile> AudioFile::Create( boost::filesystem::path const& Path, PlayMode Mode, AudioType Type)
{
	VorbisFileCache::Elem El=VorbisFileCache::Get().Load(Path);
	return std::auto_ptr<AudioFile>(El.get()?new AudioFile(El,Mode,Type):NULL);
}

bool AudioFile::IsFinishedPlaying() const
{
	return mFinishedPlaying&&mBuffer.GetSize()==0;
}

AudioFile::AudioType AudioFile::GetType() const
{
	return mType;
}

AudioBuffer& AudioFile::GetBuffer()
{
	return mBuffer;
}
