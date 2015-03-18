#include "platform/i_platform.h"
#include "engine/explode_on_death_system.h"
#include "core/i_explode_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"

namespace engine {

ExplodeOnDeathSystem::ExplodeOnDeathSystem()
    : mScene( Scene::Get() )
    , mActorFactory(ActorFactory::Get())
{

}

void ExplodeOnDeathSystem::Init()
{
}

void ExplodeOnDeathSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(), n; ( n = it, it != e ? ( ++n, true ) : false ); it = n )
    {
        Actor& actor = **it;
        Opt<IExplodeOnDeathComponent> explodeOnDeathC = actor.Get<IExplodeOnDeathComponent>();
        if (!explodeOnDeathC.IsValid())
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if(!healthC.IsValid())
        {
            continue; 
        }
        if(!healthC->IsAlive())
        {
            std::auto_ptr<Actor> explosionProjectile=mActorFactory(explodeOnDeathC->GetExplosionProjectile());
            Opt<IPositionComponent> projPositionC = explosionProjectile->Get<IPositionComponent>();
            Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
            Opt<IMoveComponent> actorMoveC = actor.Get<IMoveComponent>();
            double actorSpeedX=actorMoveC.IsValid()?actorMoveC->GetSpeedX():0.0;
            double actorSpeedY=actorMoveC.IsValid()?actorMoveC->GetSpeedY():0.0;
            Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
            double radius=collisionC.IsValid()?collisionC->GetRadius():0.0;
            double actorSpeedXNormalized=actorMoveC->GetSpeed()==0.0?0.0:actorSpeedX/abs(actorMoveC->GetSpeed());
            double actorSpeedYNormalized=actorMoveC->GetSpeed()==0.0?0.0:actorSpeedY/abs(actorMoveC->GetSpeed());
            projPositionC->SetX( actorPositionC->GetX()+actorSpeedXNormalized*radius);
            projPositionC->SetY( actorPositionC->GetY()+actorSpeedYNormalized*radius);
            mScene.AddActor(explosionProjectile.release());
            mScene.RemoveActor(it);
        }
    }
}

} // namespace engine

