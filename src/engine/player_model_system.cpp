#include "platform/i_platform.h"
#include "player_model_system.h"
#include <boost/bind.hpp>

namespace engine {

PlayerModelSystem::PlayerModelSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe(
                        boost::bind( &PlayerModelSystem::OnActorEvent, this, _1 ) );
}

void PlayerModelSystem::OnActorEvent( ActorEvent const& Evt )
{
    Actor const& actor = *Evt.mActor;
    if ( Evt.mState == ActorEvent::Removed &&
         mProgramState.mControlledActorGUID == actor.GetGUID() )
    {
        mScene.SetPlayerModels( Opt<Actor>() );
    }
}


void PlayerModelSystem::Init()
{
}


void PlayerModelSystem::Update( double DeltaTime )
{
}


} // namespace engine

