#include "platform/i_platform.h"
#include "capture_the_flag_game_mode_system.h"
#include "ui/ui.h"
#include "core/ctf_program_state.h"
#include "network/ctf_client_datas_message.h"
#include "network/messsage_holder.h"
#include "engine/soldier_spawn_system.h"
#include "engine/engine.h"
#include "engine/ctf_soldier_spawn_system.h"

namespace core {

CaptureTheFlagGameModeSystem::CaptureTheFlagGameModeSystem()
    : mScene( Scene::Get() )
    , mProgramState(core::ProgramState::Get())
    , mCtfProgramState(ctf::ProgramState::Get())
    , mCtfModel( "ctf", &RootModel::Get() )
{
}


void CaptureTheFlagGameModeSystem::Init()
{
    mOnStartGameMode=EventServer<core::StartGameModeEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnStartGameMode, this, _1 ) );
    mOnFlagStateChanged=EventServer<ctf::FlagStateChangedEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnFlagStateChanged, this, _1 ) );
    mTeamModels.clear();
    mTeamModels.push_back(new ModelValue( mCtfProgramState.GetBlueScore(), "blue", &mCtfModel ));
    mTeamModels.push_back(new ModelValue( mCtfProgramState.GetRedScore(), "red", &mCtfModel ));
}


void CaptureTheFlagGameModeSystem::Update(double DeltaTime)
{

}

void CaptureTheFlagGameModeSystem::OnStartGameMode(core::StartGameModeEvent const& Evt)
{
    if (Evt.mMode!="ctf")
    {
        return;
    }
    mCtfProgramState.ResetScores();

    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>(false);
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>(true);

    if (mProgramState.mMode==core::ProgramState::Server)
    {
        ctf::ProgramState& ctfProgramState=ctf::ProgramState::Get();
        ctfProgramState.mClientDatas.clear();
        bool isBlueTeam=true;
        for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
        {
            ctfProgramState.mClientDatas.push_back(ctf::ClientData((*i).mClientId,
                isBlueTeam?(Team::Blue):(Team::Red)));
            isBlueTeam=!isBlueTeam;
        }
        std::auto_ptr<network::ctf::ClientDatasMessage> clientDatasMsg(new network::ctf::ClientDatasMessage);
        clientDatasMsg->mClientDatas=ctfProgramState.mClientDatas;
        network::MessageHolder::Get().AddOutgoingMessage(clientDatasMsg);
    }
    mScene.Load("level2");
    Ui::Get().Load("ctf_hud");
}

void CaptureTheFlagGameModeSystem::OnFlagStateChanged(ctf::FlagStateChangedEvent const& Evt)
{
    if (Evt.mType==ctf::FlagStateChangedEvent::Scored)
    {
        if(Evt.mTeam==Team::Blue)
        {
            mCtfProgramState.mBlueScore+=5;
        }
        else
        {
            mCtfProgramState.mRedScore+=5;
        }
    }
}

} // namespace core

