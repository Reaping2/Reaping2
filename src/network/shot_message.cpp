#include "platform/i_platform.h"
#include "network/shot_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

ShotMessageSenderSystem::ShotMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ShotMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnShot=EventServer<core::ShotEvent>::Get().Subscribe( boost::bind( &ShotMessageSenderSystem::OnShot, this, _1 ) );
}


void ShotMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void ShotMessageSenderSystem::OnShot(core::ShotEvent const& Evt)
{
    std::auto_ptr<ShotMessage> shotMsg(new ShotMessage);
    shotMsg->mActorGUID=Evt.mActorGUID;
    shotMsg->mX=Evt.mPosition.x*PRECISION;
    shotMsg->mY=Evt.mPosition.y*PRECISION;
    shotMsg->mIsAlt=Evt.mIsAlt;
    mMessageHolder.AddOutgoingMessage(shotMsg);
}

ShotMessageHandlerSubSystem::ShotMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ShotMessageHandlerSubSystem::Init()
{
}

void ShotMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void ShotMessageHandlerSubSystem::Execute(Message const& message)
{
    ShotMessage const& msg=static_cast<ShotMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    EventServer<core::ShotEvent>::Get().SendEvent(core::ShotEvent(msg.mActorGUID,glm::vec2(msg.mX/PRECISION,msg.mY/PRECISION),msg.mIsAlt));
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT(network__ShotMessage, network::ShotMessage);
