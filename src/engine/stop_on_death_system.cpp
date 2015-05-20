#include "platform/i_platform.h"
#include "stop_on_death_system.h"
#include "core/i_stop_on_death_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"

namespace engine {

StopOnDeathSystem::StopOnDeathSystem()
    : mScene( Scene::Get() )
{
}


void StopOnDeathSystem::Init()
{
}


void StopOnDeathSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<IStopOnDeathComponent> stopOnDeathC=actor.Get<IStopOnDeathComponent>();
       if (!stopOnDeathC.IsValid()||stopOnDeathC->IsStopped())
       {
           continue;
       }

       Opt<IHealthComponent> healthC=actor.Get<IHealthComponent>();
       if (!healthC.IsValid())
       {
           continue;
       }
       if (healthC->IsAlive())
       {
           continue;
       }
       stopOnDeathC->SetStopped(true);
       Opt<IMoveComponent> moveC=actor.Get<IMoveComponent>();
       if(moveC.IsValid())
       {
           moveC->SetMoving(false);
           moveC->SetHeadingModifier(0);
       }
    }
}


} // namespace engine

