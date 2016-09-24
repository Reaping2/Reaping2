#include "platform/i_platform.h"
#include "remove_components_on_death_system.h"
#include "core/i_remove_components_on_death_component.h"
#include "core/i_health_component.h"

namespace engine {

RemoveComponentsOnDeathSystem::RemoveComponentsOnDeathSystem()
    : mScene( Scene::Get() )
{
}


void RemoveComponentsOnDeathSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IRemoveComponentsOnDeathComponent>().IsValid()
            && actor.Get<IHealthComponent>().IsValid(); } );
}


void RemoveComponentsOnDeathSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IRemoveComponentsOnDeathComponent> removeComponentsOnDeathC = actor->Get<IRemoveComponentsOnDeathComponent>();
        if ( !removeComponentsOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if( !healthC.IsValid() )
        {
            continue;
        }
        if( healthC->IsAlive() )
        {
            continue;
        }
        // create copy, component might drop itself
        std::vector<int32_t> const comps = removeComponentsOnDeathC->GetComponents();
        std::for_each( std::begin( comps ), std::end( comps ),
                [&]( int32_t id ) { actor->DropComponent( id ); } );
    }
}


} // namespace engine

