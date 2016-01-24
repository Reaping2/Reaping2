#include "network/revive_message.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/actor_factory.h"
#include "core/i_inventory_component.h"
#include "lifecycle_message_handler_sub_system.h"
#include "engine/soldier_spawn_system.h"
#include "core/player_controller_component.h"
#include "core/revive_event.h"
namespace network {

    ReviveMessageSenderSystem::ReviveMessageSenderSystem()
        : MessageSenderSystem()
    {
    }

    void ReviveMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        SetFrequency(10);
    }

    void ReviveMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
        //--not used: currently no revive request from client
    }


    ReviveMessageHandlerSubSystem::ReviveMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
        
    {

    }

    void ReviveMessageHandlerSubSystem::Init()
    {

    }

    void ReviveMessageHandlerSubSystem::Execute(Message const& message)
    {
        //--not used: currently no revive request from client
        ReviveMessage const& msg=static_cast<ReviveMessage const&>(message);
        L2("got revive message: senderId:%d\n",msg.mSenderId);

        Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(msg.mSenderId));
        if (!clientData.IsValid())
        {
            L1("cannot find clientdata for revive: senderId: %d\n",msg.mSenderId);
            return;
        }

        L2("found client for revive: senderId:%d\n",msg.mSenderId);
        EventServer<core::ReviveEvent>::Get().SendEvent( core::ReviveEvent( clientData ) );
        L2("end revive message: senderId:%d\n",msg.mSenderId);
    }

} // namespace engine

