#include "network/create_actor_message_sender_system.h"
#include "create_actor_message.h"
#include "core/shot_collision_component.h"
#include "set_ownership_message.h"
#include "position_message_sender_system.h"
#include "move_message_sender_system.h"
#include "orientation_message.h"
#include "heading_message.h"
#include "collision_message.h"
#include "border_message.h"
#include "fade_out_message.h"
#include "engine/system_suppressor.h"
namespace network {


CreateActorMessageSenderSystem::CreateActorMessageSenderSystem()
    : MessageSenderSystem()
{

}

void CreateActorMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &CreateActorMessageSenderSystem::OnActorEvent, this, _1 ) );
    engine::SystemSuppressor::Get().Add( engine::SystemSuppressor::SceneLoad, GetType_static() );
}

void CreateActorMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void CreateActorMessageSenderSystem::OnActorEvent( ActorEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    std::auto_ptr<CreateActorMessage> createActorMsg(
        new CreateActorMessage( Evt.mState == ActorEvent::Added ? Evt.mActor : Opt<Actor>( NULL ) ) );
    createActorMsg->mActorGUID = Evt.mActor->GetGUID();
    createActorMsg->mState = Evt.mState;
    if (createActorMsg->mState == ActorEvent::Removed)
    {
        L2( "createactormessage remove sent  with (GUID:%d)\n", createActorMsg->mActorGUID );
    }
    else if (createActorMsg->mState == ActorEvent::Added)
    {
        L2( "createactormessage add sent  with (GUID:%d)\n", createActorMsg->mActorGUID );
    }
    mMessageHolder.AddOutgoingMessage( createActorMsg );
}

} // namespace engine

