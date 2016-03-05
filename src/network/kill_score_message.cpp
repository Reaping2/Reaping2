#include "platform/i_platform.h"
#include "network/kill_score_message.h"
#include "core/player_controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

KillScoreMessageSenderSystem::KillScoreMessageSenderSystem()
    : MessageSenderSystem()
{
}


void KillScoreMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnKillScore=EventServer<engine::KillScoreEvent>::Get().Subscribe( boost::bind( &KillScoreMessageSenderSystem::OnKillScore, this, _1 ) );
}


void KillScoreMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void KillScoreMessageSenderSystem::OnKillScore(engine::KillScoreEvent const& Evt)
{
    std::auto_ptr<KillScoreMessage> killScoreMsg(new KillScoreMessage);
    killScoreMsg->mKillerGUID=Evt.mKillerGUID;
    killScoreMsg->mDeadGUID=Evt.mDeadGUID;
    mMessageHolder.AddOutgoingMessage(killScoreMsg);
}

KillScoreMessageHandlerSubSystem::KillScoreMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void KillScoreMessageHandlerSubSystem::Init()
{
}

void KillScoreMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void KillScoreMessageHandlerSubSystem::Execute(Message const& message)
{
    KillScoreMessage const& msg=static_cast<KillScoreMessage const&>(message);
    EventServer<engine::KillScoreEvent>::Get().SendEvent(engine::KillScoreEvent(msg.mKillerGUID,msg.mDeadGUID));
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT(network__KillScoreMessage, network::KillScoreMessage);
