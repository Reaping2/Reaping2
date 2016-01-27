#include "platform/i_platform.h"
#include "audio_system.h"
#include "core/i_audible_component.h"
#include "core/i_position_component.h"
#include "core/audible_event.h"
#include "core/program_state.h"
#include "i_audio.h"

namespace audio {
struct EffectInst
{
    int32_t UID;
    int32_t Id;
    glm::vec2 Pos;
    EffectInst( int32_t uid, int32_t id, glm::vec2 const& pos )
        : UID( uid ), Id( id ), Pos( pos ) {}
};


AudioSystem::AudioSystem()
    : mScene( Scene::Get() )
{
}


void AudioSystem::Init()
{
}


void AudioSystem::Update(double DeltaTime)
{
    typedef std::vector<EffectInst> Effects_t;
    static AudioPlayer& ap( AudioPlayer::Get() );
    Effects_t effects;
    auto prevEffects = ap.HaltableEffects();
    static core::ProgramState& programState( core::ProgramState::Get() );
    static platform::EventServer<core::AudibleEvent>& es( platform::EventServer<core::AudibleEvent>::Get() );
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IAudibleComponent> audibleC=actor.Get<IAudibleComponent>();
        if (!audibleC.IsValid())
        {
            continue;
        }
        Opt<IPositionComponent> posC=actor.Get<IPositionComponent>();
        glm::vec2 pos;
        if( posC.IsValid() )
        {
            pos = glm::vec2( posC->GetX(), posC->GetY() );
        }
        if( programState.mMode != core::ProgramState::Client )
        {
            std::for_each( std::begin( audibleC->GetEffects() ), std::end( audibleC->GetEffects() ),
                    []( AudibleEffectDesc& d ) { if( AudibleEffectDesc::TTL_Infinity != d.TTL ) --d.TTL; } );
        }
        std::for_each( std::begin( audibleC->GetEffects() ), std::end( audibleC->GetEffects() ),
                [&]( AudibleEffectDesc const& d ) { effects.push_back( EffectInst( d.UID, d.Id, pos ) ); } );
        if( programState.mMode == core::ProgramState::Server )
        {
            std::for_each( std::begin( audibleC->GetEffects() ), std::end( audibleC->GetEffects() ),
                    [&]( AudibleEffectDesc const& d ) { if( AudibleEffectDesc::TTL_Infinity != d.TTL && 0 >= d.TTL ) es.SendEvent( core::AudibleEvent( actor.GetGUID(), d.Id, false, false ) ); } );
        }
        audibleC->GetEffects().erase( std::remove_if( std::begin( audibleC->GetEffects() ), std::end( audibleC->GetEffects() ),
                    []( AudibleEffectDesc const& d ) { return d.TTL <= 0; } ), std::end( audibleC->GetEffects() ) );
    }
    Effects_t alleffects( effects );
    auto removedEffectsIt = std::remove_if( std::begin( prevEffects ), std::end( prevEffects ),
            [&]( int32_t i ) { return std::find_if( std::begin( effects ), std::end( effects ), [&]( EffectInst const& ei ) { return ei.UID == i; } ) == std::end( effects ); } );
    std::vector<int32_t> removedEffects( removedEffectsIt, std::end( prevEffects ) );
    prevEffects.erase( removedEffectsIt, std::end( prevEffects ) );
    std::for_each( std::begin( removedEffects ), std::end( removedEffects ),
            [&]( int32_t i ) { ap.Halt( i ); } );
    std::for_each( std::begin( effects ), std::end( effects ),
            [&]( EffectInst const& i ) { ap.Play( i.UID, i.Id, i.Pos ); } );
}


} // namespace audio

