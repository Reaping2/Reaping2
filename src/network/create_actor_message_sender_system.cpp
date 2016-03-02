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
namespace network {


    CreateActorMessageSenderSystem::CreateActorMessageSenderSystem()
        : MessageSenderSystem()
    {

    }

    void CreateActorMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &CreateActorMessageSenderSystem::OnActorEvent, this, _1 ) );
    }

    void CreateActorMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
    }

    void CreateActorMessageSenderSystem::OnActorEvent(ActorEvent const& Evt)
    {
        std::auto_ptr<CreateActorMessage> createActorMsg(
            new CreateActorMessage(Evt.mState==ActorEvent::Added?Evt.mActor:Opt<Actor>(NULL)));
        createActorMsg->mActorGUID=Evt.mActor->GetGUID();
        createActorMsg->mState=Evt.mState;
        mMessageHolder.AddOutgoingMessage(createActorMsg);
    }

} // namespace engine

