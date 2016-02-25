#include "platform/i_platform.h"
#include "network/heal_taken_message.h"
#include "core/i_health_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

HealTakenMessageSenderSystem::HealTakenMessageSenderSystem()
    : MessageSenderSystem()
{
}


void HealTakenMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnHealTaken=EventServer<core::HealTakenEvent>::Get().Subscribe( boost::bind( &HealTakenMessageSenderSystem::OnHealTaken, this, _1 ) );
}


void HealTakenMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void HealTakenMessageSenderSystem::OnHealTaken(core::HealTakenEvent const& Evt)
{
    std::auto_ptr<HealTakenMessage> healTakenMsg(new HealTakenMessage);
    healTakenMsg->mX=std::floor(Evt.mX*PRECISION);
    healTakenMsg->mY=std::floor(Evt.mY*PRECISION);
    healTakenMsg->mHeal=Evt.mHeal;
    healTakenMsg->mActorGUID=Evt.mActorGUID;
    mMessageHolder.AddOutgoingMessage(healTakenMsg);
}

HealTakenMessageHandlerSubSystem::HealTakenMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void HealTakenMessageHandlerSubSystem::Init()
{
}

void HealTakenMessageHandlerSubSystem::Execute(Message const& message)
{
    HealTakenMessage const& msg=static_cast<HealTakenMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
    if (!healthC.IsValid())
    {
        L1("heal taken on an actor withot heal component \n" );
        return;
    }
    healthC->TakeHeal(msg.mHeal);
}


} // namespace network


BOOST_CLASS_EXPORT_IMPLEMENT(network::HealTakenMessage);
