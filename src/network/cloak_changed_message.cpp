#include "platform/i_platform.h"
#include "network/cloak_changed_message.h"
#include "core/i_cloak_component.h"

namespace network {

CloakChangedMessageSenderSystem::CloakChangedMessageSenderSystem()
    : MessageSenderSystem()
{
}


void CloakChangedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnCloakChanged=EventServer<engine::CloakChangedEvent>::Get().Subscribe( boost::bind( &CloakChangedMessageSenderSystem::OnCloakChanged, this, _1 ) );
}


void CloakChangedMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void CloakChangedMessageSenderSystem::OnCloakChanged(engine::CloakChangedEvent const& Evt)
{
    std::auto_ptr<CloakChangedMessage> cloakChangedMsg(new CloakChangedMessage);
    cloakChangedMsg->mActorGUID=Evt.mActorGUID;
    cloakChangedMsg->mActivated=Evt.mActivated;
    mMessageHolder.AddOutgoingMessage(cloakChangedMsg);
}

CloakChangedMessageHandlerSubSystem::CloakChangedMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void CloakChangedMessageHandlerSubSystem::Init()
{
}

void CloakChangedMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void CloakChangedMessageHandlerSubSystem::Execute(Message const& message)
{
    CloakChangedMessage const& msg=static_cast<CloakChangedMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    Opt<ICloakComponent> cloakC=actor->Get<ICloakComponent>();
    if (cloakC.IsValid())
    {
        cloakC->SetActive(msg.mActivated);        
    }
//    EventServer<engine::CloakChangedEvent>::Get().SendEvent(engine::CloakChangedEvent(msg.mActorGUID,msg.mActivated));
}

} // namespace network

