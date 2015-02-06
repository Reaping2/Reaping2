#include "platform/i_platform.h"
#include "network/create_actor_message_handler_sub_system.h"
#include "create_actor_message.h"
namespace network {

    CreateActorMessageHandlerSubSystem::CreateActorMessageHandlerSubSystem()
        : mMessageHolder(MessageHolder::Get())
        , mActorFactory(ActorFactory::Get())
        , mScene(Scene::Get())
    {

    }

    void CreateActorMessageHandlerSubSystem::Init()
    {

    }

    void CreateActorMessageHandlerSubSystem::Execute(Message const& message)
    {
        CreateActorMessage const& msg=static_cast<CreateActorMessage const&>(message);
        L1("executing createactor: %d \n",msg.mSenderId );
        std::auto_ptr<Actor> actor(mActorFactory(msg.mActorId));
        actor->SetGUID(msg.mActorGUID);
        mScene.AddActor(actor.release());
    }

} // namespace engine

