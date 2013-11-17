#include "i_audio.h"

AudioBuffer::AudioBuffer( size_t NumChannels )
: mChannels(NumChannels)
{
	mData.resize(mChannels);
}

AudioBuffer::Buffer_t const& AudioBuffer::GetData( size_t ChannelNum ) const
{
	assert(ChannelNum<mChannels);
	return mData.at(ChannelNum);
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

void AudioBuffer::Write(AudioBuffer& Other, size_t Size)
{
	if(!Size)
		return;
	size_t CommonChannels=std::min<size_t>(mChannels,Other.mChannels);
	if(!CommonChannels)
		return;
	size_t S=mData[0].size();
	for(size_t i=0;i<CommonChannels;++i)
	{
		Buffer_t& Buf=mData[i];
		Buffer_t& OBuf=Other.mData[i];
		Buf.resize(S+Size);
		std::copy(OBuf.begin(),OBuf.begin()+Size,Buf.begin()+S);
	}
	Other.Read(Size);
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
