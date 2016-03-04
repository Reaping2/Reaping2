#include "platform/i_platform.h"
#include "soldier_auto_revive_system.h"
#include "core/i_health_component.h"
#include "core/revive_event.h"
#include "core/player_controller_component.h"

namespace engine {

SoldierAutoReviveSystem::SoldierAutoReviveSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mAutoReviveTextModel( GetStringFunc( this, &SoldierAutoReviveSystem::GetText ), "auto_revive_text", &RootModel::Get() )
    , mAutoReviveText("")
    , mSecsToRevive(-1)
{
}


void SoldierAutoReviveSystem::Init()
{
    mSoldierAutoReviveMES=map::SoldierAutoReviveMapElementSystem::Get();
    if (mProgramState.mMode!=core::ProgramState::Server)
    {
        mOnSecsToRevive=EventServer<engine::SecsToReviveEvent>::Get().Subscribe( boost::bind( &SoldierAutoReviveSystem::OnSecsToRevive, this, _1 ) );
    }
    if (mProgramState.mMode!=core::ProgramState::Client)
    {
        mOnActorEvent=EventServer<ActorEvent>::Get().Subscribe( boost::bind( &SoldierAutoReviveSystem::OnActorEvent, this, _1 ) );
    }
}


void SoldierAutoReviveSystem::Update(double DeltaTime)
{
    if (mProgramState.mGameState!=core::ProgramState::Running)
    {
        return;
    }
    if (mProgramState.mMode!=core::ProgramState::Client)
    {
        double secsToRevive=mSoldierAutoReviveMES->GetSecsToRevive();
        double currTime=glfwGetTime();
        for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
        {
            core::ClientData& clientData=*i;
            auto deathTimeIt = mTimesOfDeath.find( i->mClientActorGUID );
            Opt<Actor> player = mScene.GetActor( i->mClientActorGUID );
            if ( deathTimeIt != mTimesOfDeath.end() && deathTimeIt->second + secsToRevive <= currTime )
            {
                if (clientData.mConnected)
                {
                    if( player.IsValid() )
                    {
                        Opt<PlayerControllerComponent> playerControllerC(player->Get<PlayerControllerComponent>());
                        if (playerControllerC.IsValid())
                        {
                            playerControllerC->SetEnabled(false);
                            playerControllerC->mActive=false;
                        }
                    }
                    EventServer<core::ReviveEvent>::Get().SendEvent( core::ReviveEvent( Opt<core::ClientData>(&*i) ) );
                    mTimesOfDeath.erase( deathTimeIt );
                }
            }
            else if( !player.IsValid() && clientData.mConnected && clientData.mReady )
            {
                EventServer<core::ReviveEvent>::Get().SendEvent( core::ReviveEvent( Opt<core::ClientData>(&*i) ) );
            }
        }
    }

    if (mSecsToRevive>0)
    {
        mSecsToRevive-=DeltaTime;
        mAutoReviveText=
            "Revive in "+boost::lexical_cast<std::string>(
            std::max(   //no negative reviv shown. 
            0.0,std::ceil(mSecsToRevive)
            ))+" secs no worries!";
    }
    else
    {
        mAutoReviveText.clear();
    }
}

std::string const& SoldierAutoReviveSystem::GetText()
{
    return mAutoReviveText;
}

void SoldierAutoReviveSystem::OnActorEvent(ActorEvent const& Evt)
{
    if (Evt.mState==ActorEvent::Died)
    {
        Opt<core::ClientData> clientData(mProgramState.FindClientDataByActorGUID(Evt.mActor->GetGUID()));
        if (clientData.IsValid())
        {
            Opt<IHealthComponent> healthC(Evt.mActor->Get<IHealthComponent>());
            if ( healthC.IsValid() )
            {
                mTimesOfDeath[ Evt.mActor->GetGUID() ] = healthC->GetTimeOfDeath();
            }
            EventServer<engine::SecsToReviveEvent>::Get().SendEvent( engine::SecsToReviveEvent(clientData->mClientId,mSoldierAutoReviveMES->GetSecsToRevive() ) );
        }
    }
}

void SoldierAutoReviveSystem::OnSecsToRevive(engine::SecsToReviveEvent const& Evt)
{
    if (Evt.mClientId==mProgramState.mClientId)
    {
        mSecsToRevive=Evt.mSecsToRevive;
    }
}


} // namespace engine

