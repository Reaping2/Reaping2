#include "platform/i_platform.h"
#include "particle_system.h"
#include "core/i_emitter_component.h"
#include "core/i_position_component.h"
#include "engine/system_factory.h"
#include "particle_engine.h"
#include "core/i_move_component.h"

namespace render {

ParticleSystem::ParticleSystem()
    : mScene( Scene::Get() )
{
}


void ParticleSystem::Init()
{
}


void ParticleSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IEmitterComponent> emitterC = actor.Get<IEmitterComponent>();
        Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
        Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
        glm::vec2 speed(0);
        if (moveC.IsValid()&&moveC->IsMoving()&&!moveC->IsRooted())
        {
            speed = glm::vec2(moveC->GetSpeedX()*DeltaTime, moveC->GetSpeedY()*DeltaTime);
        }
        if ( !emitterC.IsValid() || !positionC.IsValid() )
        {
            continue;
        }
        emitterC->Update( DeltaTime );
        std::vector<int32_t> const& emitted = emitterC->GetEmitTypes();
        for( std::vector<int32_t>::const_iterator ie = emitted.begin(), ee = emitted.end(); ie != ee; ++ie )
        {
            static ParticleEngine& pe( ParticleEngine::Get() );
            pe.AddParticle( *ie, glm::vec2( positionC->GetX(), positionC->GetY() ), speed, positionC->GetOrientation() );
        }
        emitterC->Emitted( emitted );
    }
}

REGISTER_SYSTEM( ParticleSystem );

} // namespace render

