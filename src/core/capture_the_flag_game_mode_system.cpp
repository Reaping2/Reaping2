#include "platform/i_platform.h"
#include "capture_the_flag_game_mode_system.h"
#include "ui/ui.h"
#include "core/ctf_program_state.h"
#include "network/ctf_client_datas_message.h"
#include "network/messsage_holder.h"
#include "engine/soldier_spawn_system.h"
#include "engine/engine.h"
#include "engine/ctf_soldier_spawn_system.h"
#include "i_team_component.h"
#include "platform/event.h"
#include "engine/show_text_event.h"

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
    mOnScore=EventServer<engine::ScoreEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnScore, this, _1 ) );
    mTeamModels.clear();
    mTeamModels.push_back(new ModelValue( mCtfProgramState.GetBlueScore(), "blue", &mCtfModel ));
    mTeamModels.push_back(new ModelValue( mCtfProgramState.GetRedScore(), "red", &mCtfModel ));
}


void CaptureTheFlagGameModeSystem::Update(double DeltaTime)
{
    if (mProgramState.mMode==core::ProgramState::Server)
    {
        if (mCtfProgramState.mBlueScore>=20)
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(40000,"BLUE TEAM IS VICTORIOUS!"));
        }
        if (mCtfProgramState.mRedScore>=20)
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(40000,"RED TEAM IS VICTORIOUS!"));
        }
    }
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
        EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Welcome to HELL Bitches!"));
        EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Capture that flag"));
        EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"GL HF!"));
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
            mCtfProgramState.mRedScore+=5;
           EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Red team scored!"));
        }
        else
        {
            mCtfProgramState.mBlueScore+=5;
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Blue team scored!"));
        }
    }
    else if (Evt.mType==ctf::FlagStateChangedEvent::Captured)
    {
        if(Evt.mTeam==Team::Blue)
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Blue flag captured!!"));
        }
        else
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Red flag captured!"));
        }
    }
    else if (Evt.mType==ctf::FlagStateChangedEvent::Returned)
    {
        if(Evt.mTeam==Team::Blue)
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Blue flag returned!"));
        }
        else
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(4,"Red flag returned!"));
        }
    }
}

void CaptureTheFlagGameModeSystem::OnScore(engine::ScoreEvent const& Evt)
{
    Opt<Actor> actor=mScene.GetActor(Evt.mActorGUID);
    Opt<ITeamComponent> actorTeamC(actor->Get<ITeamComponent>());
    if (!actorTeamC.IsValid())
    {
        return;
    }
    if (actorTeamC->GetTeam()==Team::Blue)
    {
        mCtfProgramState.mRedScore+=1;
    }
    else 
    {
        mCtfProgramState.mBlueScore+=1;
    }
}


} // namespace core
