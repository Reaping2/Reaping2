#include "platform/i_platform.h"
#include "target_holder_system.h"
#include "core/i_target_holder_component.h"

namespace engine {

TargetHolderSystem::TargetHolderSystem()
    : mScene( Scene::Get() )
{
}


void TargetHolderSystem::Init()
{
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &TargetHolderSystem::OnActorEvent, this, _1 ) );
}

void TargetHolderSystem::OnActorEvent(ActorEvent const& Evt)
{
    if(Evt.mState==ActorEvent::Removed)
    {
        mRemovedActorGUIDs.insert(Evt.mActor->GetGUID());
    }
}

void TargetHolderSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<ITargetHolderComponent> targetHolderC=actor.Get<ITargetHolderComponent>();
       if (!targetHolderC.IsValid()||targetHolderC->GetTarget()==NULL)
       {
           continue;
       }
       if (mRemovedActorGUIDs.find(targetHolderC->GetTarget()->GetGUID())!=mRemovedActorGUIDs.end())
       {
           targetHolderC->SetTarget(NULL);
       }
    }
    mRemovedActorGUIDs.clear();
}


} // namespace engine

