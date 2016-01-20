#include "platform/i_platform.h"
#include "audio_system.h"
#include "core/i_audible_component.h"
#include "core/i_position_component.h"
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

typedef std::vector<EffectInst> Effects_t;
Effects_t mPrevEffects;

AudioSystem::AudioSystem()
    : mScene( Scene::Get() )
{
}


void AudioSystem::Init()
{
}


void AudioSystem::Update(double DeltaTime)
{
    Effects_t effects;
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
       std::for_each( audibleC->GetEffects().begin(), audibleC->GetEffects().end(),
               [&]( AudibleEffectDesc const& d ) { effects.emplace_back( d.UID, d.Id, pos ); } );
    }
    Effects_t alleffects( effects );
    auto removedEffectsIt = std::remove_if( std::begin( mPrevEffects ), std::end( mPrevEffects ),
        [&]( EffectInst const& i ) { return std::find_if( std::begin( effects ), std::end( effects ), [&]( EffectInst const& ei ) { return ei.UID == i.UID; } ) == std::end( effects ); } );
    Effects_t removedEffects( removedEffectsIt, std::end( mPrevEffects ) );
    mPrevEffects.erase( removedEffectsIt, std::end( mPrevEffects ) );
    auto oldEffectsIt = std::remove_if( std::begin( effects ), std::end( effects ),
        [&]( EffectInst const& i ) { return std::find_if( std::begin( mPrevEffects ), std::end( mPrevEffects ), [&]( EffectInst const& pi ) { return pi.UID == i.UID; } ) != std::end( mPrevEffects ); } );
    Effects_t updatedEffects( oldEffectsIt, std::end( effects ) );
    effects.erase( oldEffectsIt, std::end( effects ) );
    static AudioPlayer& ap( AudioPlayer::Get() );
    std::for_each( std::begin( effects ), std::end( effects ),
        [&]( EffectInst const& i ) { ap.Play( i.UID, i.Id, i.Pos ); } );
    std::swap( alleffects, mPrevEffects );
//    ap.Update( effects, updatedEffects, removedEffects );
}


} // namespace audio

