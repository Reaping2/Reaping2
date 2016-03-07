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

namespace engine {

LocalSystem::LocalSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mLocalStartModel( VoidFunc( this, &LocalSystem::OnLocalStart ), "local.start", &RootModel::Get() )
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
    core::ClientData clientData = core::ClientData( mProgramState.mClientId, mProgramState.mClientName );
    clientData.mConnected = true;
    mProgramState.mClientDatas.push_back( clientData );
    EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent( engine::ClientDatasChangedEvent() );
    Ui::Get().Load( "soldier_properties" );
}

void LocalSystem::OnSoldierPropertiesReady( SoldierPropertiesReadyEvent const& Evt )
{
    mScene.SelectLevel( "level1" ); // TODO: temporary level selection should be implemented in local too
    Opt<core::ClientData> clientData( mProgramState.FindClientDataByClientId( mProgramState.mClientId ) );
    clientData->mSoldierProperties = mProgramState.mSoldierProperties;
    mProgramState.mGameState = core::ProgramState::Running;
    EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( "ffa" ) );
}


} // namespace engine

