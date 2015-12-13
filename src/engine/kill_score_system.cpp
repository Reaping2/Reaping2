#include "platform/i_platform.h"
#include "kill_score_system.h"
#include "core/player_controller_component.h"
#include "core/team_component.h"
#include "show_text_event.h"

namespace engine {

KillScoreSystem::KillScoreSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mScoreModel( "score", &RootModel::Get() )
    , mKill(0)
    , mDeath(0)
    , mScore(0)
{
}


void KillScoreSystem::Init()
{
    mOnKillScore=EventServer<engine::KillScoreEvent>::Get().Subscribe( boost::bind( &KillScoreSystem::OnKillScore, this, _1 ) );
    mScoreModels.clear();
    mScoreModels.push_back(new ModelValue( GetIntFunc( this, &KillScoreSystem::GetKill ), "kill", &mScoreModel ));
    mScoreModels.push_back(new ModelValue( GetIntFunc( this, &KillScoreSystem::GetDeath ), "death", &mScoreModel ));
    mScoreModels.push_back(new ModelValue( GetIntFunc( this, &KillScoreSystem::GetScore ), "score", &mScoreModel ));

}


void KillScoreSystem::Update(double DeltaTime)
{
    Opt<core::ClientData> clientData(mProgramState.FindClientDataByClientId(mProgramState.mClientId));
    if(clientData.IsValid())
    {
        mKill=clientData->mKill;
        mDeath=clientData->mDeath;
        mScore=clientData->mScore;
    }
}

void KillScoreSystem::OnKillScore(engine::KillScoreEvent const& Evt)
{
    Opt<Actor> killer=mScene.GetActor(Evt.mKillerGUID);
    Opt<Actor> dead=mScene.GetActor(Evt.mDeadGUID);
    if (!killer.IsValid())
    {
        L1("cannot find killer actor with GUID: (%s) %d \n",__FUNCTION__,Evt.mKillerGUID );
        return;
    }
    if (!dead.IsValid())
    {
        L1("cannot find dead actor with GUID: (%s) %d \n",__FUNCTION__,Evt.mDeadGUID );
        return;
    }
    Opt<PlayerControllerComponent> killerPCC=killer->Get<PlayerControllerComponent>();
    Opt<PlayerControllerComponent> deadPCC=dead->Get<PlayerControllerComponent>();
    Opt<core::ClientData> killerClientData;
    if (killerPCC.IsValid())
    {
        killerClientData=mProgramState.FindClientDataByClientId(killerPCC->mControllerId);
    }
    Opt<core::ClientData> deadClientData;
    if (deadPCC.IsValid())
    {
        deadClientData=mProgramState.FindClientDataByClientId(deadPCC->mControllerId);
    }

    if (deadClientData.IsValid())
    {
        ++deadClientData->mDeath;   //he is dead that's for sure
    }

    if (killerClientData.IsValid())
    {
        Opt<TeamComponent> killerTeamC=killer->Get<TeamComponent>();
        Opt<TeamComponent> deadTeamC=dead->Get<TeamComponent>();
        if (killerTeamC.IsValid()&&deadTeamC.IsValid())
        {
            if (killerTeamC->GetTeam()==deadTeamC->GetTeam())
            {
                --killerClientData->mKill; //you got one less kill, that's a punishment for TK
                //--killerClientData->mScore; //your score is decreased too. naughty naughty!
            }
            else
            {
                ++killerClientData->mKill; 
                ++killerClientData->mScore;
            }
        }
        else if (deadPCC.IsValid())
        {
            ++killerClientData->mKill;
            ++killerClientData->mScore;
        }
    }

    if (killerClientData.IsValid()&&deadClientData.IsValid())
    {
        EventServer<engine::ShowTextEvent>::Get().SendEvent(engine::ShowTextEvent(2.5,killerClientData->mClientName+" reaped "+deadClientData->mClientName+"!"));
    }
}

int32_t const& KillScoreSystem::GetKill()
{
    return mKill;
}

int32_t const& KillScoreSystem::GetDeath()
{
    return mDeath;
}

int32_t const& KillScoreSystem::GetScore()
{
    return mScore;
}


} // namespace engine

