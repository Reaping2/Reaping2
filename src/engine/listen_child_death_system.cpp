#include "platform/i_platform.h"
#include "listen_child_death_system.h"
#include "core/i_listen_child_death_component.h"
#include "engine.h"

namespace engine {

ListenChildDeathSystem::ListenChildDeathSystem()
    : mScene( Scene::Get() )
{
}


void ListenChildDeathSystem::Init()
{
    mRemovedActorsSystem=Engine::Get().GetSystem<RemovedActorsSystem>();
}


void ListenChildDeathSystem::Update(double DeltaTime)
{
//     for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
//     {
//        Actor& actor = **it;
//        Opt<IListenChildDeathComponent> listenChildDeathC=actor.Get<IListenChildDeathComponent>();
//        Opt<Actor> killerOfChild(mScene.GetActor(listenChildDeathC->GetKillerOfChildId()));
//        if (!listenChildDeathC.IsValid())
//        {
//            continue;
//        }
//     }
}


} // namespace engine

