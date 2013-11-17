#ifndef INCLUDED_AUDIO_AUDIO_BUFFER_H
#define INCLUDED_AUDIO_AUDIO_BUFFER_H

class AudioBuffer
{
public:
	typedef std::vector<float> Buffer_t;
private:
	boost::ptr_vector<Buffer_t> mData;
	const size_t mChannels;
public:
	AudioBuffer(size_t NumChannels);
	size_t GetChannels()const;
	Buffer_t const& GetData(size_t ChannelNum)const;
	size_t const GetSize()const;
	void Write(float** Data, size_t Size);
	void Write(AudioBuffer& Other, size_t Size);
	void Read(size_t Size);

};

#endif//INCLUDED_AUDIO_AUDIO_BUFFER_H
