#include "platform/i_platform.h"
#include "network/show_text_message_message.h"
#include "platform/event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

ShowTextMessageMessageSenderSystem::ShowTextMessageMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ShowTextMessageMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnShowText=EventServer<engine::ShowTextEvent>::Get().Subscribe( boost::bind( &ShowTextMessageMessageSenderSystem::OnShowText, this, _1 ) );
}


void ShowTextMessageMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void ShowTextMessageMessageSenderSystem::OnShowText(engine::ShowTextEvent const& Evt)
{
    std::auto_ptr<ShowTextMessageMessage> showTextMessageMsg(new ShowTextMessageMessage);
    showTextMessageMsg->mSeconds=Evt.mSeconds;
    showTextMessageMsg->mText=Evt.mText;
    mMessageHolder.AddOutgoingMessage(showTextMessageMsg);
}

ShowTextMessageMessageHandlerSubSystem::ShowTextMessageMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ShowTextMessageMessageHandlerSubSystem::Init()
{
}

void ShowTextMessageMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void ShowTextMessageMessageHandlerSubSystem::Execute(Message const& message)
{
    ShowTextMessageMessage const& msg=static_cast<ShowTextMessageMessage const&>(message);
    EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(msg.mSeconds,msg.mText));    
}

} // namespace network


BOOST_CLASS_EXPORT_IMPLEMENT(network::ShowTextMessageMessage);
