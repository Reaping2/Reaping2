#include "i_audio.h"

AudioBuffer::AudioBuffer( size_t NumChannels )
: mChannels(NumChannels)
{
	mData.resize(mChannels);
}

float const* AudioBuffer::GetData( size_t ChannelNum ) const
{
	assert(ChannelNum<mChannels);
	return &*mData.at(ChannelNum).begin();
}

size_t const AudioBuffer::GetSize() const
{
	return mChannels?mData.at(0).size():0;
}

void AudioBuffer::Write( float** Data, size_t Size )
{
	if(!mChannels)
		return;
	size_t S=mData[0].size();
	for(size_t i=0;i<mChannels;++i)
	{
		Buffer_t& Buf=mData[i];
		Buf.resize(S+Size);
		std::copy(Data[i],Data[i]+Size,Buf.begin()+S);
	}
}

void AudioBuffer::Read( size_t Size )
{
	for(boost::ptr_vector<Buffer_t>::iterator i=mData.begin(),e=mData.end();i!=e;++i)
	{
		Buffer_t& Buf=*i;
		Buf.erase(Buf.begin(),Buf.begin()+Size);
	}
}

size_t AudioBuffer::GetChannels() const
{
	return mChannels;
}

size_t AudioBuffer::CopyFrom(AudioBuffer const& Source,size_t PosInSource, size_t Size, bool ContinueFromStart)
{
	size_t CommonChannels=std::min<size_t>(mChannels,Source.mChannels);
	if(!CommonChannels)
		return 0;
	size_t S=mData[0].size();
	size_t SourceSize=Source.mData[0].size();
	size_t i=0;
	size_t const SizeToCopyUntilEnd=std::min<size_t>(Size,SourceSize-PosInSource);
	size_t const SizeToCopyFromBegin=ContinueFromStart?(Size-SizeToCopyUntilEnd):0;
	size_t const SizeToCopyTotal=SizeToCopyUntilEnd+SizeToCopyFromBegin;
	for(;i<CommonChannels;++i)
	{
		Buffer_t& Buf=mData[i];
		float const* OBuf=&Source.mData[i][0];
		Buf.resize(S+SizeToCopyTotal);
		std::copy(OBuf+PosInSource,OBuf+PosInSource+SizeToCopyUntilEnd,Buf.begin()+S);
		if(!SizeToCopyFromBegin)continue;
		std::copy(OBuf,OBuf+SizeToCopyFromBegin,Buf.begin()+S+SizeToCopyUntilEnd);
	}
	for(;i<mChannels;++i)
	{
		Buffer_t& Buf=mData[i];
		Buf.resize(S+SizeToCopyTotal);
	}
	return SizeToCopyTotal;
}
