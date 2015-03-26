#include "platform/i_platform.h"
#include "target_holder_system.h"
#include "core/i_target_holder_component.h"
#include "engine.h"
#include "items/inventory_system.h"

namespace engine {

TargetHolderSystem::TargetHolderSystem()
    : mScene( Scene::Get() )
{
}


void TargetHolderSystem::Init()
{
    mRemovedActorsSystem=Engine::Get().GetSystem<RemovedActorsSystem>();
}


void TargetHolderSystem::Update(double DeltaTime)
{
//     for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
//     {
//        Actor& actor = **it;
//        Opt<ITargetHolderComponent> targetHolderC=actor.Get<ITargetHolderComponent>();
//        Opt<Actor> currentTarget(mScene.GetActor(targetHolderC->GetTargetId()));
//        if (!targetHolderC.IsValid()||!currentTarget.IsValid())
//        {
//            continue;
//        }
//     }
}


} // namespace engine

