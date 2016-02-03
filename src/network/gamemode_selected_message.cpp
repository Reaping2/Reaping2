#include "platform/i_platform.h"
#include "network/gamemode_selected_message.h"
#include "network/load_clientlist_event.h"

namespace network {

GamemodeSelectedMessageSenderSystem::GamemodeSelectedMessageSenderSystem()
    : MessageSenderSystem()
{
}


void GamemodeSelectedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnGamemodeSelectedEvent =
        EventServer<core::GamemodeSelectedEvent>::Get().Subscribe( boost::bind(&GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent, this, _1 ) );
}


void GamemodeSelectedMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent(core::GamemodeSelectedEvent const & evt)
{
    std::auto_ptr<GamemodeSelectedMessage> msg(new GamemodeSelectedMessage);
    msg->mGameMode = evt.mGameMode;
    msg->mOriginator = mProgramState.mClientId;
    mMessageHolder.AddOutgoingMessage(msg);
    std::cerr << " .... GamemodeSelectedMessage client id " << mProgramState.mClientId << "\n";
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
    if ( mProgramState.mMode == ProgramState::Client  )
    {
        std::cerr << "Receiving gamemodeselected... my client id " << mProgramState.mClientId << " received cid : " << msg.mOriginator << "\n";
        if ( mProgramState.mClientId != msg.mOriginator )
        {
            std::cerr << "Receiving gamemodeselected...and processing\n";
            network::LoadClientlistEvent event;
            event.mGameMode = msg.mGameMode;
            EventServer<network::LoadClientlistEvent>::Get().SendEvent(event);
        }
    }
    else if ( mProgramState.mMode == ProgramState::Server )
    {
        std::auto_ptr<GamemodeSelectedMessage> reply(new GamemodeSelectedMessage);
        reply->mGameMode = msg.mGameMode;
        reply->mOriginator = msg.mOriginator;
        mMessageHolder.AddOutgoingMessage( reply );
    }
}

} // namespace network

