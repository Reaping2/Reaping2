#include "platform/i_platform.h"
#include "network/health_message.h"
#include "core/i_health_component.h"

namespace network {

HealthMessageSenderSystem::HealthMessageSenderSystem()
    : MessageSenderSystem()
{
    //This one is not used yet you should wire it before use
}


void HealthMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
        //This one is not used yet you should wire it before use
}


void HealthMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
        //This one is not used yet you should wire it before use
}

HealthMessageHandlerSubSystem::HealthMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void HealthMessageHandlerSubSystem::Init()
{
}

void HealthMessageHandlerSubSystem::Execute(Message const& message)
{
    HealthMessage const& msg=static_cast<HealthMessage const&>(message);
    L1("stashing %s: actorGUID %d hp: %d maxHp-base: %d\n",__FUNCTION__,msg.mActorGUID,msg.mHP,msg.mMaxHP );

    mPendingHealthMessages.push_back(msg);
}

void HealthMessageHandlerSubSystem::Update(double DeltaTime)
{
    for(PendingHealthMessages_t::iterator i=mPendingHealthMessages.begin();i!=mPendingHealthMessages.end();)
    {
        HealthMessage const& msg=*i;
        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (%s) %d (will try again later) \n",__FUNCTION__,msg.mActorGUID );
            ++i;
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if (!healthC.IsValid())
        {
            L1("no health_component found guid:%s\n",msg.mActorGUID);
            i=mPendingHealthMessages.erase(i);
            continue;
        }
        L1("executing %s: actorGUID %d \n",__FUNCTION__,msg.mActorGUID );

        healthC->SetHP(msg.mHP);
        healthC->GetMaxHP().mBase.Set(msg.mMaxHP);
        healthC->GetMaxHP().mPercent.Set(msg.mMaxHPPercent/PRECISION);
        healthC->GetMaxHP().mFlat.Set(msg.mMaxHPFlat);
        i=mPendingHealthMessages.erase(i);
    }
}

std::auto_ptr<HealthMessage> HealthMessageSenderSystem::GenerateHealthMessage(Actor &actor)
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (!healthC.IsValid())
    {
        return std::auto_ptr<HealthMessage>();
    }
    std::auto_ptr<HealthMessage> healthMsg(new HealthMessage);
    healthMsg->mActorGUID=actor.GetGUID();
    healthMsg->mHP=healthC->GetHP();
    healthMsg->mMaxHP=healthC->GetMaxHP().mBase.Get();
    healthMsg->mMaxHPPercent=std::floor(healthC->GetMaxHP().mPercent.Get()*PRECISION);
    healthMsg->mMaxHPFlat=healthC->GetMaxHP().mFlat.Get();
    return healthMsg;
}

} // namespace network

