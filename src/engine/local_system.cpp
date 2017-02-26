#include "platform/i_platform.h"
#include "local_system.h"
#include "ui/ui.h"
#include "core/start_game_mode_event.h"
#include "client_datas_changed_event.h"
#include "engine.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "core/actor_factory.h"
#include "core/program_state.h"
#include "core/ctf_program_state.h"
#include "network/client_ready_event.h"
#include "network/client_list_changed_event.h"

namespace engine {

LocalSystem::LocalSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mLocalStartModel( VoidFunc( this, &LocalSystem::OnLocalStart ), "local.start", &RootModel::Get() )
    , mLifecycleModel( "lifecycle", &RootModel::Get() )
    , mHostModel( VoidFunc( this, &LocalSystem::Host ), "host", &mLifecycleModel )
{
    mOnSoldierPropertiesReady = EventServer<SoldierPropertiesReadyEvent>::Get().Subscribe( boost::bind( &LocalSystem::OnSoldierPropertiesReady, this, _1 ) );
}


void LocalSystem::Init()
{
    Ui::Get().Load( "start" );
    mKeyboard =::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
}


void LocalSystem::Update( double DeltaTime )
{
    if( mKeyboard->GetKey( GLFW_KEY_C ).State == KeyState::Typed )
    {
        std::ostringstream oss;
        eos::portable_oarchive oa( oss );
        ActorList_t& actorlist = Scene::Get().GetActors();
        oa& actorlist;
        std::string astr( oss.str() );

        std::istringstream iss( astr );
        eos::portable_iarchive ia( iss );
        ActorList_t actorlist2;
        ia >> actorlist2;

        Scene::Get().SetPlayerModels( Opt<Actor>() );
        Scene::Get().ClearActors();
        Scene::Get().SetActors( actorlist2 );
        Scene::Get().SetPlayerModels( Scene::Get().GetActor( core::ProgramState::Get().mControlledActorGUID ) );
    }

}

void LocalSystem::OnLocalStart()
{
    mProgramState.mClientId = 0;
    mProgramState.mClientName = "kvakmama";
    mProgramState.mClientDatas.clear();
    mProgramState.mIsHost = true;
    int32_t controlledLocalPlayerId = 1;    // TODO: change when local multiplayer is possible
    core::ClientData clientData = core::ClientData( mProgramState.mClientId, mProgramState.mClientName, controlledLocalPlayerId );
    clientData.mConnected = true;
    mProgramState.mClientDatas.push_back( clientData );
    EventServer<network::ClientListChangedEvent>::Get().SendEvent( mProgramState.mClientDatas );
    EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent( engine::ClientDatasChangedEvent() );
    Ui::Get().Load( "soldier_properties" );
}

void LocalSystem::OnSoldierPropertiesReady( SoldierPropertiesReadyEvent const& Evt )
{
    Opt<core::ClientData> clientData( mProgramState.FindClientDataByClientId( mProgramState.mClientId ) );
    clientData->mSoldierProperties = mProgramState.mSoldierProperties;
    network::ClientReadyEvent event;
    event.mClientId = clientData->mClientId;
    event.mClientName = clientData->mClientName;
    EventServer<network::ClientReadyEvent>::Get().SendEvent( event );
    Ui::Get().Load( "select_game_mode" );
}


void LocalSystem::Host()
{
    EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( mProgramState.mGameMode ) );
}

} // namespace engine

