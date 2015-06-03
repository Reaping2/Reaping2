#include "network/revive_message.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/actor_factory.h"
#include "core/i_inventory_component.h"
#include "lifecycle_message_handler_sub_system.h"
#include "engine/soldier_spawn_system.h"
#include "core/player_controller_component.h"
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
        L2("handle revive request!\n");
        //TODO: might need optimization
        Opt<Actor> actor=mScene.GetActor(mProgramState.mControlledActorGUID);
        if (!actor.IsValid())
        {
            L1("actor not valid!\n");
            return;
        }
        Opt<PlayerControllerComponent> playerControllerC(actor->Get<PlayerControllerComponent>());
        if (!playerControllerC.IsValid())
        {
            L1("playercontroller not valid!\n");
            return;
        }
        if (!playerControllerC->mReviveTyped)
        {
            return;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if (healthC.IsValid()&&!healthC->IsAlive())
        {
            std::auto_ptr<ReviveMessage> reviveMsg(new ReviveMessage);
            mMessageHolder.AddOutgoingMessage(reviveMsg);
            L2("revive message sent clientId: %d!\n",mProgramState.mClientId);
        }
        else
        {
            L1("%s health is not available, or actor still alive:%d\n",__FUNCTION__,actor->GetGUID());
        }
        playerControllerC->mReviveTyped=false;
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
        ReviveMessage const& msg=static_cast<ReviveMessage const&>(message);
        L2("got revive message: senderId:%d\n",msg.mSenderId);

        Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(msg.mSenderId));
        if (!clientData.IsValid())
        {
            L1("cannot find clientdata for revive: senderId: %d\n",msg.mSenderId);
            return;
        }

        L2("found client for revive: senderId:%d\n",msg.mSenderId);
        std::auto_ptr<Actor> player(engine::SoldierSpawnSystem::Get()->Spawn(*clientData));       
        mScene.AddActor(player.release());
       
        L2("end revive message: senderId:%d\n",msg.mSenderId);
    }

} // namespace engine

