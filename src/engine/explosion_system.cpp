#include "platform/i_platform.h"
#include "explosion_system.h"
#include "core/i_explosion_component.h"
#include "core/i_collision_component.h"

namespace engine {

ExplosionSystem::ExplosionSystem()
    : mScene( Scene::Get() )
{
}


void ExplosionSystem::Init()
{
}


void ExplosionSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<IExplosionComponent> explosionC=actor.Get<IExplosionComponent>();
       if (!explosionC.IsValid())
       {
           continue;
       }
       Opt<ICollisionComponent> collisionC=actor.Get<ICollisionComponent>();
       if (!collisionC.IsValid())
       {
           continue;
       }
       double radius=collisionC->GetRadius();
       radius+=DeltaTime*explosionC->GetScaleSpeed();
       collisionC->SetRadius(std::min(radius,explosionC->GetMaxRadius()));
    }
}


} // namespace engine

