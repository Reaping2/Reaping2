#include "platform/i_platform.h"
#include "network/set_ownership_message.h"
#include "core/player_controller_component.h"
#include "core/i_inventory_component.h"

namespace network {

SetOwnershipMessageSenderSystem::SetOwnershipMessageSenderSystem()
    : MessageSenderSystem()
{
}


void SetOwnershipMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnSoldierCreated=EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &SetOwnershipMessageSenderSystem::OnSoldierCreated, this, _1 ) );
}


void SetOwnershipMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void SetOwnershipMessageSenderSystem::OnSoldierCreated(engine::SoldierCreatedEvent const& Evt)
{
    std::auto_ptr<SetOwnershipMessage> setOwnershipMsg(new SetOwnershipMessage);
    setOwnershipMsg->mActorGUID=Evt.mActor->GetGUID();
    setOwnershipMsg->mClientId=Evt.mClientData.mClientId;
    mMessageHolder.AddOutgoingMessage(setOwnershipMsg);
}

SetOwnershipMessageHandlerSubSystem::SetOwnershipMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void SetOwnershipMessageHandlerSubSystem::Init()
{
}

void SetOwnershipMessageHandlerSubSystem::Update(double DeltaTime)
{
    PendingMessageHandlerSubSystem::Update(DeltaTime);
}

bool SetOwnershipMessageHandlerSubSystem::ProcessPending(Message const& message)
{
    SetOwnershipMessage const& msg=static_cast<SetOwnershipMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID); //guaranteed
    L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );
    Opt<PlayerControllerComponent> playerControllerC(actor->Get<PlayerControllerComponent>());
    if (!playerControllerC.IsValid())
    {
        L1("setownership is called on an actor that is not playerControllable \n" );
        return true;
    }

    playerControllerC->mControllerId=msg.mClientId;

    if (msg.mClientId==mProgramState.mClientId)
    {
        L1("thats my id! actorguid: %d \n",msg.mActorGUID);
        Opt<Actor> oldActor=mScene.GetActor(mProgramState.mControlledActorGUID);
        if (oldActor.IsValid())
        {
            oldActor->Get<PlayerControllerComponent>()->SetEnabled(false);
            oldActor->Get<PlayerControllerComponent>()->mActive=false;
        }
        mProgramState.mControlledActorGUID=msg.mActorGUID;
        playerControllerC->SetEnabled(true);
        playerControllerC->mActive=true;
        mScene.SetPlayerModels(actor);
    }
    Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(msg.mClientId));
    if (clientData.IsValid())
    {
        clientData->mClientActorGUID=msg.mActorGUID;
    }
    return true;
}

} // namespace network

