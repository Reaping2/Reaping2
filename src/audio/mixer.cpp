#include "i_audio.h"
#include "platform/model_value.h"
#include "platform/i_platform.h"

namespace {
float left( float x )
{
    if( x < 0.0f )
    {
        return 1.f - 1.f / ( 2.f + 2.f * std::abs( x / 1e3 ) );
    }
    else
    {
        return 1.f / ( 2.f + 2.f * ( x / 1e3 ) );
    }
}
float right( float x )
{
    return left( -x );
}
}

void Mixer::Mix( AudioBuffer& Dest, AudioFiles_t& Files, size_t const Size )
{
    if( !Size )
    {
        return;
    }
    glm::vec2 playerPos;
    ModelValue const& PlayerModel = RootModel::Get()["player"];
    if( PlayerModel.IsValid() )
    {
        ModelValue const& mx = PlayerModel["x"];
        ModelValue const& my = PlayerModel["y"];
        if ( mx.IsValid() && my.IsValid() )
        {
            playerPos.x = double( mx );
            playerPos.y = double( my );
        }
    }
    size_t const DestChannels = Dest.GetChannels();
    float** Tmp = new float*[DestChannels];

    for( size_t i = 0; i < DestChannels; ++i )
    {
        Tmp[i] = new float[Size];
        memset( Tmp[i], 0, sizeof( float )*Size );
    }

    for( AudioFiles_t::iterator i = Files.begin(), e = Files.end(); i != e; ++i )
    {
        AudioFile& f = *i;
        float Weight = mAudioTypeRelativeWeights[f.GetType()];
        AudioBuffer& Buf = f.GetBuffer();
        float l = 1.0, r = 1.0;
        if( f.GetType() == audio::Effect )
        {
            glm::vec2 const& pos = f.GetPosition();
            glm::vec2 const dif = pos - playerPos;
            const float DistWeight = std::max( 1.f / ( 1.f + glm::dot( dif, dif ) / 1e6 ) - 0.1, 0.0 );
            Weight *= DistWeight;
            l = left( dif.x );
            r = right( dif.x );
        }
        Weight *= f.GetVolume();
        size_t CommonChannels = std::min<size_t>( Buf.GetChannels(), DestChannels );
        for( size_t Ch = 0; Ch < CommonChannels; ++Ch )
        {
            float const* SrcBuf = Buf.GetData( Ch );
            float* DstBuf = Tmp[Ch];
            for( size_t j = 0; j < Size; ++j )
            {
                ( *DstBuf++ ) += ( Ch == 0 ? l : r ) * Weight * ( *SrcBuf++ );
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
    mAudioTypeRelativeWeights[audio::Music] = 0.35f;
    mAudioTypeRelativeWeights[audio::Effect] = 0.2f;
    mAudioTypeRelativeWeights[audio::Speech] = 0.45f;
}

