#include "platform/i_platform.h"
#include "engine/explode_on_death_system.h"
#include "core/i_explode_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"

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
            projPositionC->SetX( actorPositionC->GetX() );
            projPositionC->SetY( actorPositionC->GetY() );
            mScene.AddActor(explosionProjectile.release());
            mScene.RemoveActor(it);
        }
    }
}

} // namespace engine

