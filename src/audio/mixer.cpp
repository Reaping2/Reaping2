#include "i_audio.h"

void Mixer::Mix( AudioBuffer& Dest, AudioFiles_t& Files, size_t const Size )
{
    if( !Size )
    {
        return;
    }
    size_t const DestChannels = Dest.GetChannels();
    float** Tmp = new float*[DestChannels];

    for( size_t i = 0; i < DestChannels; ++i )
    {
        Tmp[i] = new float[Size];
        memset( Tmp[i], 0, sizeof( float )*Size );
    }

    size_t NumFilesInCateg[AudioFile::NumTypes] = {0};
    for( AudioFiles_t::iterator i = Files.begin(), e = Files.end(); i != e; ++i )
    {
        AudioFile& f = *i;
        ++NumFilesInCateg[f.GetType()];
    }
    for( AudioFiles_t::iterator i = Files.begin(), e = Files.end(); i != e; ++i )
    {
        AudioFile& f = *i;
        const float Weight = mAudioTypeRelativeWeights[f.GetType()] / NumFilesInCateg[f.GetType()];
        AudioBuffer& Buf = f.GetBuffer();
        size_t CommonChannels = std::min<size_t>( Buf.GetChannels(), DestChannels );
        for( size_t Ch = 0; Ch < CommonChannels; ++Ch )
        {
            float const* SrcBuf = Buf.GetData( Ch );
            float* DstBuf = Tmp[Ch];
            for( size_t j = 0; j < Size; ++j )
            {
                ( *DstBuf++ ) += Weight * ( *SrcBuf++ );
            }
        }
        Buf.Read( Size );
    }

    Dest.Write( Tmp, Size );

    for( size_t i = 0; i < DestChannels; ++i )
    {
        delete[] Tmp[i];
    }
    delete[] Tmp;
}

Mixer::Mixer()
{
    // todo: make it adjustable
    mAudioTypeRelativeWeights[AudioFile::Music] = 0.00f;//0.35f;
    mAudioTypeRelativeWeights[AudioFile::Effect] = 0.2f;
    mAudioTypeRelativeWeights[AudioFile::Speech] = 0.45f;
}

