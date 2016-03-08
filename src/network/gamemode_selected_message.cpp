#include "platform/i_platform.h"
#include "network/gamemode_selected_message.h"
#include "network/load_clientlist_event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "lifecycle_message.h"

namespace network {

GamemodeSelectedMessageSenderSystem::GamemodeSelectedMessageSenderSystem()
    : MessageSenderSystem()
{
}


void GamemodeSelectedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnGamemodeSelectedEvent =
        EventServer<core::GamemodeSelectedEvent>::Get().Subscribe( boost::bind( &GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent, this, _1 ) );
}


void GamemodeSelectedMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void GamemodeSelectedMessageSenderSystem::OnGamemodeSelectedEvent( core::GamemodeSelectedEvent const& evt )
{
    std::auto_ptr<GamemodeSelectedMessage> msg( new GamemodeSelectedMessage );
    msg->mGameMode = evt.mGameMode;
    msg->mOriginator = mProgramState.mClientId;
    mMessageHolder.AddOutgoingMessage( msg );
}

GamemodeSelectedMessageHandlerSubSystem::GamemodeSelectedMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void GamemodeSelectedMessageHandlerSubSystem::Init()
{
}

void GamemodeSelectedMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void GamemodeSelectedMessageHandlerSubSystem::Execute( Message const& message )
{
    GamemodeSelectedMessage const& msg = static_cast<GamemodeSelectedMessage const&>( message );
    if ( mProgramState.mMode == ProgramState::Client )
    {
        mProgramState.mGameMode = msg.mGameMode;
    }
    if ( mProgramState.mMode == ProgramState::Server )
    {
        mProgramState.mGameMode = msg.mGameMode;
        std::auto_ptr<GamemodeSelectedMessage> gameModeSelectedMsg( new GamemodeSelectedMessage );
        gameModeSelectedMsg->mGameMode = msg.mGameMode;
        gameModeSelectedMsg->mOriginator = mProgramState.mClientId;
        mMessageHolder.AddOutgoingMessage( gameModeSelectedMsg );
        std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
        lifecycleMsg->mState = LifecycleMessage::ClientList;
        lifecycleMsg->mGameMode = msg.mGameMode;
        lifecycleMsg->mClientId = -1;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
        lifecycleMsg.reset( new LifecycleMessage );
        lifecycleMsg->mState = LifecycleMessage::SelectLevel;
        lifecycleMsg->mGameMode = msg.mGameMode;
        lifecycleMsg->mClientId = msg.mOriginator;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
    }
}

} // namespace network
REAPING2_CLASS_EXPORT_IMPLEMENT( network__GamemodeSelectedMessage, network::GamemodeSelectedMessage )

