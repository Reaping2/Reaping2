#include "platform/i_platform.h"
#include "network/team_switch_request_message.h"

namespace network {

TeamSwitchRequestMessageSenderSystem::TeamSwitchRequestMessageSenderSystem()
    : MessageSenderSystem()
{
}


void TeamSwitchRequestMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void TeamSwitchRequestMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

TeamSwitchRequestMessageHandlerSubSystem::TeamSwitchRequestMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void TeamSwitchRequestMessageHandlerSubSystem::Init()
{
}

void TeamSwitchRequestMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void TeamSwitchRequestMessageHandlerSubSystem::Execute(Message const& message)
{
    TeamSwitchRequestMessage const& msg=static_cast<TeamSwitchRequestMessage const&>(message);
}

} // namespace network

