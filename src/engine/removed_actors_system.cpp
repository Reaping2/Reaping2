#include "platform/i_platform.h"
#include "removed_actors_system.h"

namespace engine {

RemovedActorsSystem::RemovedActorsSystem()
    : mScene( Scene::Get() )
{
}


void RemovedActorsSystem::Init()
{
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &RemovedActorsSystem::OnActorEvent, this, _1 ) );
}


void RemovedActorsSystem::Update( double DeltaTime )
{
    mRemovedActorGUIDs.clear();
}

RemovedActorsSystem::RemovedActorsGUIDs_t const& RemovedActorsSystem::GetRemovedActors() const
{
    return mRemovedActorGUIDs;
}

void RemovedActorsSystem::OnActorEvent( ActorEvent const& Evt )
{
    if( Evt.mState == ActorEvent::Removed )
    {
        mRemovedActorGUIDs.insert( Evt.mActor->GetGUID() );
    }
}


} // namespace engine

