#include "platform/i_platform.h"
#include "network/flash_message.h"
#include "core/i_position_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

FlashMessageSenderSystem::FlashMessageSenderSystem()
    : MessageSenderSystem()
{
}


void FlashMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnFlashEvent=EventServer<engine::FlashEvent>::Get().Subscribe( boost::bind( &FlashMessageSenderSystem::OnFlashEvent, this, _1 ) );
}


void FlashMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void FlashMessageSenderSystem::OnFlashEvent(engine::FlashEvent const& Evt)
{
    std::auto_ptr<FlashMessage> flashMsg(new FlashMessage);
    flashMsg->mActorGUID=Evt.mActorGUID;
    flashMsg->mOriginalX=std::floor(Evt.mOriginalX*PRECISION);
    flashMsg->mOriginalY=std::floor(Evt.mOriginalY*PRECISION);
    flashMsg->mX=std::floor(Evt.mX*PRECISION);
    flashMsg->mY=std::floor(Evt.mY*PRECISION);
    mMessageHolder.AddOutgoingMessage(flashMsg);
}

FlashMessageHandlerSubSystem::FlashMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void FlashMessageHandlerSubSystem::Init()
{
}

void FlashMessageHandlerSubSystem::Execute(Message const& message)
{
    FlashMessage const& msg=static_cast<FlashMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    Opt<IPositionComponent> positionC=actor->Get<IPositionComponent>();
    if (!positionC.IsValid())
    {
        return;
    }
    positionC->SetX(msg.mX/PRECISION);
    positionC->SetY(msg.mY/PRECISION);
}


} // namespace network


BOOST_CLASS_EXPORT_IMPLEMENT(network::FlashMessage);
