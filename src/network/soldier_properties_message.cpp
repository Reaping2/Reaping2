#include "platform/i_platform.h"
#include "network/soldier_properties_message.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/max_health_buff.h"
#include "engine/buffs_engine/max_health_buff_sub_system.h"
#include "core/i_health_component.h"
#include "health_message.h"

namespace network {

SoldierPropertiesMessageSenderSystem::SoldierPropertiesMessageSenderSystem()
    : MessageSenderSystem()
    , mClientReadyModel( VoidFunc( this, &SoldierPropertiesMessageSenderSystem::OnClientReady ), "client.ready", &RootModel::Get() )
{
    mOnActorEvent=EventServer<ActorEvent>::Get().Subscribe( boost::bind( &SoldierPropertiesMessageSenderSystem::OnActorEvent, this, _1 ) );
}

void SoldierPropertiesMessageSenderSystem::OnActorEvent(ActorEvent const& Evt)
{
    if(mProgramState.mMode==ProgramState::Server
        &&Evt.mState==ActorEvent::Added)
    {
        Opt<core::ClientData> clientData(mProgramState.FindClientDataByActorGUID(Evt.mActor->GetGUID()));
        if (!clientData.IsValid())
        {
            return;
        }
        Opt<IBuffHolderComponent> buffHolderC = Evt.mActor->Get<IBuffHolderComponent>();
        if(buffHolderC.IsValid())
        {
            std::auto_ptr<Buff> buff(core::BuffFactory::Get()(AutoId("move_speed_buff")));
            MoveSpeedBuff* moveSpeedBuff= (MoveSpeedBuff*)buff.get();
            moveSpeedBuff->SetAutoRemove(false);
            moveSpeedBuff->SetFlatBonus(clientData->mSoldierProperties.mMoveSpeed*20);
            buffHolderC->AddBuff(buff);

            buff=core::BuffFactory::Get()(AutoId("max_health_buff"));
            MaxHealthBuff* maxHealthBuff=(MaxHealthBuff*)buff.get();
            maxHealthBuff->SetFlatBonus(clientData->mSoldierProperties.mHealth*15);
            maxHealthBuff->SetAutoRemove(false);
            buffHolderC->AddBuff(buff);
            engine::MaxHealthBuffSubSystem::RecalculateBuffs(*Evt.mActor);
            Opt<IHealthComponent> healthC=Evt.mActor->Get<IHealthComponent>();
            healthC->SetHP(healthC->GetMaxHP().Get());
            L1("setting health to hp:%d, maxHP calculated:%d guid: %d\n",healthC->GetHP(),healthC->GetMaxHP().Get(),Evt.mActor->GetGUID());
            mMessageHolder.AddOutgoingMessage(network::HealthMessageSenderSystem::GenerateHealthMessage(*Evt.mActor));
        }

    }
}

void SoldierPropertiesMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void SoldierPropertiesMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void SoldierPropertiesMessageSenderSystem::OnClientReady()
{
    if (mProgramState.mMode!=ProgramState::Server)
    {
        std::auto_ptr<SoldierPropertiesMessage> soldierPorpertiesMessage(new SoldierPropertiesMessage);
        soldierPorpertiesMessage->mSoldierProperties=mProgramState.mSoldierProperties;
        mMessageHolder.AddOutgoingMessage(soldierPorpertiesMessage);
    }

}

SoldierPropertiesMessageHandlerSubSystem::SoldierPropertiesMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void SoldierPropertiesMessageHandlerSubSystem::Init()
{
}

void SoldierPropertiesMessageHandlerSubSystem::Execute(Message const& message)
{
    SoldierPropertiesMessage const& msg=static_cast<SoldierPropertiesMessage const&>(message);
    L1("executing soldierProperties from id: %d \n",msg.mSenderId );
    L2("got revive message: senderId:%d\n",msg.mSenderId);

    Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(msg.mSenderId));
    if (!clientData.IsValid())
    {
        L1("cannot find clientdata for for properties: senderId: %d\n",msg.mSenderId);
        return;
    }

    clientData->mSoldierProperties=msg.mSoldierProperties;
    L1("**** Client: %s properties arrived. Ready to fight!!! **** from id: %d \n", clientData->mClientName.c_str(),msg.mSenderId );
    L1("   MoveSpeed:%d\n   Health:%d\n   Accuracy:%d\n", msg.mSoldierProperties.mMoveSpeed, msg.mSoldierProperties.mHealth, msg.mSoldierProperties.mAccuracy );

}


} // namespace network

