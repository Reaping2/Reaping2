#include "network/orientation_message.h"
#include "core/i_position_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "platform/settings.h"

namespace network {


void OrientationMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateOrientationMessage( actor ) );
}


void OrientationMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateOrientationMessage( actor ) );
}

OrientationMessageSenderSystem::OrientationMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "orientation" ) )
{
}

void OrientationMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.orientation", 0.01 ) );
}

void OrientationMessageSenderSystem::Update( double DeltaTime )
{
    ActorTimerMessageSenderSystem::Update( DeltaTime );
    if (!IsTime())
    {
        return;
    }
}

std::auto_ptr<OrientationMessage> OrientationMessageSenderSystem::GenerateOrientationMessage( Actor& actor )
{
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    if ( !positionC.IsValid() )
    {
        return std::auto_ptr<OrientationMessage>();
    }
    std::auto_ptr<OrientationMessage> orientationMessage( new OrientationMessage );
    orientationMessage->mOrientation = std::floor( positionC->GetOrientation() * PRECISION );
    orientationMessage->mActorGUID = actor.GetGUID();
    return orientationMessage;
}

OrientationMessageHandlerSubSystem::OrientationMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void OrientationMessageHandlerSubSystem::Init()
{

}

void OrientationMessageHandlerSubSystem::Execute( Message const& message )
{
    OrientationMessage const& msg = static_cast<OrientationMessage const&>( message );

    if ( mProgramState.mMode == ProgramState::Server
         || ( mProgramState.mMode == ProgramState::Client && msg.mActorGUID != mProgramState.mControlledActorGUID ) )
    {
        Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
        if ( !actor.IsValid() )
        {
            L1( "cannot find actor with GUID: (that is not possible) %d \n", msg.mActorGUID );
            return;
        }

        Opt<IPositionComponent> positionC( actor->Get<IPositionComponent>() );
        if ( !positionC.IsValid() )
        {
            L1( "position is called on an actor that has no position_component \n" );
            return;
        }
        positionC->SetOrientation( msg.mOrientation / PRECISION );
    }
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__OrientationMessage, network::OrientationMessage );
