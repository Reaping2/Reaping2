#ifndef INCLUDED_AUDIO_AUDIO_BUFFER_H
#define INCLUDED_AUDIO_AUDIO_BUFFER_H

class AudioBuffer
{
    typedef std::vector<float> Buffer_t;
    typedef boost::ptr_vector<Buffer_t> Buffers_t;
    Buffers_t mData;
    const size_t mChannels;
public:
    AudioBuffer( size_t NumChannels );
    size_t GetChannels()const;
    float const* GetData( size_t ChannelNum )const;
    size_t const GetSize()const;
    void Write( float** Data, size_t Size );
    void Read( size_t Size );
    size_t CopyFrom( AudioBuffer const& Source, size_t PosInSource, size_t Size, bool ContinueFromStart );
};

#endif//INCLUDED_AUDIO_AUDIO_BUFFER_H
