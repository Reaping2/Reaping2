#include "platform/i_platform.h"
#include "local_system.h"
#include "ui/ui.h"
#include "core/start_game_mode_event.h"
#include "client_datas_changed_event.h"

namespace engine {

LocalSystem::LocalSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mLocalStartModel( VoidFunc( this, &LocalSystem::OnLocalStart ), "local.start", &RootModel::Get())
{
    mOnSoldierPropertiesReady= EventServer<SoldierPropertiesReadyEvent>::Get().Subscribe( boost::bind( &LocalSystem::OnSoldierPropertiesReady, this, _1 ) );
}


void LocalSystem::Init()
{
    Ui::Get().Load("start");
}


void LocalSystem::Update(double DeltaTime)
{
}

void LocalSystem::OnLocalStart()
{
    mProgramState.mClientId=0;
    mProgramState.mClientName="kvakmama";
    mProgramState.mClientDatas.clear();
    mProgramState.mClientDatas.push_back(core::ClientData(mProgramState.mClientId,mProgramState.mClientName));
    EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent(engine::ClientDatasChangedEvent());
    Ui::Get().Load("soldier_properties");
}

void LocalSystem::OnSoldierPropertiesReady(SoldierPropertiesReadyEvent const& Evt)
{
    mScene.SelectLevel("level1");   // TODO: temporary level selection should be implemented in local too
    Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(mProgramState.mClientId));
    clientData->mSoldierProperties=mProgramState.mSoldierProperties;
    EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( "ffa" ));
}


} // namespace engine

