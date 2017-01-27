#include "platform/i_platform.h"
#include "particle_system.h"
#include "core/i_emitter_component.h"
#include "core/i_position_component.h"
#include "engine/system_factory.h"
#include "particle_engine.h"
#include "core/i_move_component.h"
#include "engine/engine.h"
#include "engine/activity_system.h"

namespace render {

ParticleSystem::ParticleSystem()
    : mScene( Scene::Get() )
{
}


void ParticleSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IEmitterComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}


void ParticleSystem::Update( double DeltaTime )
{
    static auto activityS = engine::Engine::Get().GetSystem<engine::ActivitySystem>();
    for( auto actor: activityS->GetActiveActors() )
    {
        Opt<IEmitterComponent> emitterC = actor->Get<IEmitterComponent>();
        if (!emitterC.IsValid())
        {
            continue;
        }
        Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
        if( !positionC.IsValid() )
        {
            continue;
        }
        Opt<IMoveComponent> moveC = actor->Get<IMoveComponent>();
        glm::vec2 distance(0);
        if ( moveC.IsValid() && moveC->IsMoving() && !moveC->IsRooted() )
        {
            distance = glm::vec2(moveC->GetSpeedX()*DeltaTime, moveC->GetSpeedY()*DeltaTime);
        }
        emitterC->Update( DeltaTime );
        std::vector<int32_t> const& emitted = emitterC->GetEmitTypes();
        for( std::vector<int32_t>::const_iterator ie = emitted.begin(), ee = emitted.end(); ie != ee; ++ie )
        {
            static ParticleEngine& pe( ParticleEngine::Get() );
            pe.AddParticle( *ie, glm::vec2( positionC->GetX(), positionC->GetY() ), distance, positionC->GetOrientation() );
        }
        emitterC->Emitted( emitted );
    }
}

REGISTER_SYSTEM( ParticleSystem );

} // namespace render

