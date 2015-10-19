#include "platform/i_platform.h"
#include "local_system.h"
#include "ui/ui.h"
#include "core/start_game_mode_event.h"

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
}


void LocalSystem::Update(double DeltaTime)
{
}

void LocalSystem::OnLocalStart()
{
    mProgramState.mClientId=0;
    mProgramState.mClientName="local_joe";
    mProgramState.mClientDatas.clear();
    mProgramState.mClientDatas.push_back(core::ClientData(mProgramState.mClientId,mProgramState.mClientName));
    Ui::Get().Load("soldier_properties");
}

void LocalSystem::OnSoldierPropertiesReady(SoldierPropertiesReadyEvent const& Evt)
{
    Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(mProgramState.mClientId));
    clientData->mSoldierProperties=mProgramState.mSoldierProperties;
    EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( "ffa" ));
}


} // namespace engine

