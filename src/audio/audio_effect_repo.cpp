#include "i_audio.h"

namespace audio {

AudioEffectRepo::AudioEffectRepo()
    : Repository<AudioEffect>( mDefaultEffect )
    , mDefaultEffect( "", false, Effect, PlayOnce, 0.0 )
{
    Load();
}

void AudioEffectRepo::Load()
{
    AutoFile F = Filesys::Get().Open( "sounds/effects.json" );
    if( !F.get() )
    {
        return;
    }
    JsonReader Reader( *F );
    if( !Reader.IsValid() )
    {
        return;
    }
    Json::Value& Root = Reader.GetRoot();
    if( !Root.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
    {
        Json::Value& v = *i;
        if( !v.isObject() )
        {
            continue;
        }
        std::string Key, Value;
        if( !Json::GetStr( v["id"], Key ) || !Json::GetStr( v["file"], Value ) )
        {
            continue;
        }
        int32_t Interruptable = 0;
        Json::GetInt( v["interruptable"], Interruptable );
        int32_t Type = audio::Effect;
        Json::GetInt( v["type"], Type );
        int32_t Mode = audio::PlayOnce;
        Json::GetInt( v["mode"], Mode );
        double Volume = 1.0;
        Json::GetDouble( v["volume"], Volume );
        int32_t id = AutoId( Key );
        mElements.insert( id, new AudioEffect( boost::filesystem::path( Value ), Interruptable, ( audio::AudioType )Type, ( audio::PlayMode ) Mode, Volume ) );
    }
}

}

