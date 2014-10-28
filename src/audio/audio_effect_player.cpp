#include "i_audio.h"

AudioEffectPlayer::AudioEffectPlayer()
{
    Load();
    mOnAudibleEventId = EventServer<AudibleEvent>::Get().Subscribe( boost::bind( &AudioEffectPlayer::OnAudibleEvent, this, _1 ) );
}

void AudioEffectPlayer::OnAudibleEvent( AudibleEvent const& Evt )
{
    Events_t::const_iterator i = mEvents.find( Evt.mId );
    if( i == mEvents.end() )
    {
        return;
    }
    AudioPlayer::Get().Play( i->second, AudioFile::Effect );
}

void AudioEffectPlayer::Load()
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
        mEvents[AutoId( Key )] = boost::filesystem::path( Value );
    }
}

