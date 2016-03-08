#include "platform/i_platform.h"
#include "notify_parent_on_death_system.h"
#include "core/i_notify_parent_on_death_component.h"
#include "engine.h"
#include "core/i_listen_child_death_component.h"
#include "core/i_health_component.h"

namespace engine {

NotifyParentOnDeathSystem::NotifyParentOnDeathSystem()
    : mScene( Scene::Get() )
{
}


void NotifyParentOnDeathSystem::Init()
{
    mRemovedActorsSystem = Engine::Get().GetSystem<RemovedActorsSystem>();
}


void NotifyParentOnDeathSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<INotifyParentOnDeathComponent> notifyParentOnDeathC = actor.Get<INotifyParentOnDeathComponent>();
        if ( !notifyParentOnDeathC.IsValid() )
        {
            continue;
        }
        Opt<Actor> parent( mScene.GetActor( notifyParentOnDeathC->GetParentGUID() ) );
        Opt<Actor> killer( mScene.GetActor( notifyParentOnDeathC->GetKillerGUID() ) );

        if( !parent.IsValid() )
        {
            continue;
        }

        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if( !healthC.IsValid() || healthC->IsAlive() )
        {
            continue;
        }
        if( !killer.IsValid() )
        {
            continue;
        }
        Opt<IListenChildDeathComponent> listenChildDeathC = parent->Get<IListenChildDeathComponent>();
        if ( !listenChildDeathC.IsValid() )
        {
            continue;
        }
        listenChildDeathC->SetKillerOfChildGUID( killer->GetGUID() );
    }
}


} // namespace engine

