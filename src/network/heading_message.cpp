#include "network/heading_message.h"
#include "core/i_move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "platform/settings.h"

namespace network {


void HeadingMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateHeadingMessage( actor ) );
}


void HeadingMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateHeadingMessage( actor ) );
}

HeadingMessageSenderSystem::HeadingMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "heading" ) )
{

}

void HeadingMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.heading", 0.01 ) );
}

void HeadingMessageSenderSystem::Update( double DeltaTime )
{
    ActorTimerMessageSenderSystem::Update( DeltaTime );
    if (!IsTime())
    {
        return;
    }
}

std::auto_ptr<HeadingMessage> HeadingMessageSenderSystem::GenerateHeadingMessage( Actor& actor )
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( !moveC.IsValid() )
    {
        return std::auto_ptr<HeadingMessage>();
    }
    std::auto_ptr<HeadingMessage> headingMsg( new HeadingMessage );
    headingMsg->mHeading = std::floor( moveC->GetHeading() * PRECISION );
    headingMsg->mActorGUID = actor.GetGUID();
    return headingMsg;
}

HeadingMessageHandlerSubSystem::HeadingMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void HeadingMessageHandlerSubSystem::Init()
{

}

void HeadingMessageHandlerSubSystem::Execute( Message const& message )
{
    HeadingMessage const& msg = static_cast<HeadingMessage const&>( message );

    if ( mProgramState.mMode == ProgramState::Server
         || ( mProgramState.mMode == ProgramState::Client && msg.mActorGUID != mProgramState.mControlledActorGUID ) )
    {
        Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
        if ( !actor.IsValid() )
        {
            L1( "cannot find actor with GUID: (that is not possible) %d \n", msg.mActorGUID );
            return;
        }

        Opt<IMoveComponent> moveC = actor->Get<IMoveComponent>();
        if ( !moveC.IsValid() )
        {
            L1( "heading is called on an actor that has no heading_component \n" );
            return;
        }
        moveC->SetHeading( msg.mHeading / PRECISION );
    }
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__HeadingMessage, network::HeadingMessage );
