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
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IRemoveOnDeathComponent>().IsValid()
            && actor.Get<IHealthComponent>().IsValid(); } );
}

void RemoveOnDeathSystem::Update( double DeltaTime )
{
    std::vector<int32_t> removeActorUIDs;
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IRemoveOnDeathComponent> removeOnDeathC = actor->Get<IRemoveOnDeathComponent>();
        if ( !removeOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if( !healthC.IsValid() || healthC->IsAlive() )
        {
            continue;
        }
        double RemainingTime = removeOnDeathC->GetRemainingTime() - DeltaTime;
        removeOnDeathC->SetRemainingTime( RemainingTime );
        if( RemainingTime <= 0 )
        {
            removeActorUIDs.push_back( actor->GetGUID() );
        }
    }
    for (auto uid : removeActorUIDs)
    {
        mScene.RemoveActor( uid );
    }
}

} // namespace engine

