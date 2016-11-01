#include "platform/i_platform.h"
#include "network/lifecycle_sender_system.h"
#include "lifecycle_message.h"
#include "core/program_state.h"
#include "engine/engine.h"
#include "core/level_selection_system.h"

namespace network {

LifecycleSenderSystem::LifecycleSenderSystem()
    : mMessageHolder( MessageHolder::Get() )
    , mLifecycleModel( "lifecycle", &RootModel::Get() )
    , mHostModel( VoidFunc( this, &LifecycleSenderSystem::Host ), "host", &mLifecycleModel )
    , mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}

void LifecycleSenderSystem::Init()
{
}

void LifecycleSenderSystem::Update( double DeltaTime )
{

}

void LifecycleSenderSystem::Host()
{
    if( !core::ProgramState::Get().mClientConnected )
    {
        L1( "Need to connect first before hosting a game!\n" );
        return;
    }
    auto levelSelectionSystem = engine::Engine::Get().GetSystem<core::LevelSelectionSystem>();
    if ( !levelSelectionSystem.IsValid() )
    {
        L1("failed to retrieve LevelSelectionSystem\n");
        return;
    }
    std::auto_ptr<LifecycleMessage> msg( new LifecycleMessage );
    msg->mState = LifecycleMessage::Start;
    msg->mGameMode = mProgramState.mGameMode;
    msg->mSelectedLevel = levelSelectionSystem->GetSelectedLevel();
    mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( msg.release() ) );
}




} // namespace engine

