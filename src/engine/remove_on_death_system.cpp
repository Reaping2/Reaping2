#include "platform/i_platform.h"
#include "engine/remove_on_death_system.h"
#include "core/i_remove_on_death_component.h"
#include "core/i_health_component.h"

namespace engine {

RemoveOnDeathSystem::RemoveOnDeathSystem()
    : mScene( Scene::Get() )
{

}

void RemoveOnDeathSystem::Init()
{
}

void RemoveOnDeathSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(), n; ( n = it, it != e ? ( ++n, true ) : false ); it = n )
    {
        Actor& actor = **it;
        Opt<IRemoveOnDeathComponent> removeOnDeathC = actor.Get<IRemoveOnDeathComponent>();
        if ( !removeOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if( !healthC.IsValid() || healthC->IsAlive() )
        {
            continue;
        }
        double RemainingTime = removeOnDeathC->GetRemainingTime() - DeltaTime;
        removeOnDeathC->SetRemainingTime( RemainingTime );
        if( RemainingTime <= 0 )
        {
            mScene.RemoveActor( it );
        }
    }
}

} // namespace engine

