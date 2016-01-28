#include "platform/i_platform.h"
#include "network/gamemode_selected_message.h"

namespace network {

GamemodeSelectedMessageSenderSystem::GamemodeSelectedMessageSenderSystem()
    : MessageSenderSystem()
{
    mOnGamemodeSelectedEvent =
        EventServer<core::GamemodeSelectedEvent>::Get().Subscribe( boost::bind(&GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent, this, _1 ) );
}


void GamemodeSelectedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void GamemodeSelectedMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent(core::GamemodeSelectedEvent const & evt)
{
    //TODO: do we get back the message? if yes, then we should avoid the infinite loop of : msg rec -> event -> send msg -> rec msg ...
    std::auto_ptr<GamemodeSelectedMessage> msg(new GamemodeSelectedMessage);
    msg->mGameMode = evt.mGameMode;
    mMessageHolder.AddOutgoingMessage(msg);
}

GamemodeSelectedMessageHandlerSubSystem::GamemodeSelectedMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void GamemodeSelectedMessageHandlerSubSystem::Init()
{
}

void GamemodeSelectedMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void GamemodeSelectedMessageHandlerSubSystem::Execute(Message const& message)
{
    GamemodeSelectedMessage const& msg=static_cast<GamemodeSelectedMessage const&>(message);
    // TODO: now everyone got it I hope. send gamemodeselectedevent and the waiting page should switch to the client list
    core::GamemodeSelectedEvent event;
    event.mGameMode = msg.mGameMode;
    EventServer<core::GamemodeSelectedEvent>::Get().SendEvent(event);
}

} // namespace network

