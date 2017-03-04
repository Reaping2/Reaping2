#include "platform/i_platform.h"
#include "network/set_team_message.h"
#include "core/i_team_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

SetTeamMessageSenderSystem::SetTeamMessageSenderSystem()
    : MessageSenderSystem()
{
}


void SetTeamMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnSoldierCreated = EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &SetTeamMessageSenderSystem::OnSoldierCreated, this, _1 ) );
    mOnFlagCreated = EventServer<engine::FlagCreatedEvent>::Get().Subscribe( boost::bind( &SetTeamMessageSenderSystem::OnFlagCreated, this, _1 ) );
}


void SetTeamMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void SetTeamMessageSenderSystem::OnSoldierCreated( engine::SoldierCreatedEvent const& Evt )
{
    if (Evt.mState != engine::SoldierCreatedEvent::Finished)
    {
        return;
    }
    mMessageHolder.AddOutgoingMessage( GenerateSetTeamMessage( *Evt.mActor ) );
}
void SetTeamMessageSenderSystem::OnFlagCreated( engine::FlagCreatedEvent const& Evt )
{
    mMessageHolder.AddOutgoingMessage( GenerateSetTeamMessage( *Evt.mActor ) );
}

SetTeamMessageHandlerSubSystem::SetTeamMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void SetTeamMessageHandlerSubSystem::Init()
{
}

void SetTeamMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool SetTeamMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    SetTeamMessage const& msg = static_cast<SetTeamMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID ); //guaranteed
    L2( "executing %s: actorGUID %d \n", __FUNCTION__, msg.mActorGUID );
    Opt<ITeamComponent> teamC = actor->Get<ITeamComponent>();
    if ( !teamC.IsValid() )
    {
        L1( "teamMessage arrived for actor without i_team_component actorGUID %d \n", msg.mActorGUID );
        return true;
    }
    teamC->SetTeam( msg.mTeam );
    return true;
}

std::auto_ptr<SetTeamMessage> SetTeamMessageSenderSystem::GenerateSetTeamMessage( Actor& actor )
{
    Opt<ITeamComponent> teamC = actor.Get<ITeamComponent>();
    if ( !teamC.IsValid() )
    {
        return std::auto_ptr<SetTeamMessage>();
    }
    std::auto_ptr<SetTeamMessage> setTeamMsg( new SetTeamMessage );
    setTeamMsg->mActorGUID = actor.GetGUID();
    setTeamMsg->mTeam = teamC->GetTeam();
    return setTeamMsg;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__SetTeamMessage, network::SetTeamMessage );
