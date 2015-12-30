#include "platform/i_platform.h"
#include "particle_system.h"
#include "core/i_emitter_component.h"
#include "core/i_position_component.h"
#include "engine/system_factory.h"
#include "particle_engine.h"

namespace render {

ParticleSystem::ParticleSystem()
    : mScene( Scene::Get() )
{
}


void ParticleSystem::Init()
{
}


void ParticleSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<IEmitterComponent> emitterC=actor.Get<IEmitterComponent>();
       Opt<IPositionComponent> pos=actor.Get<IPositionComponent>();
       if (!emitterC.IsValid() || !pos.IsValid())
       {
           continue;
       }
       emitterC->Update( DeltaTime );
       if( !emitterC->IsEmitting() )
       {
           continue;
       }
       static ParticleEngine& pe( ParticleEngine::Get() );
       pe.AddParticle( emitterC->GetEmitType(), glm::vec2( pos->GetX(), pos->GetY() ) );
    }
}

REGISTER_SYSTEM( ParticleSystem );

} // namespace render

