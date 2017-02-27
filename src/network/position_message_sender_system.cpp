#include "network/position_message_sender_system.h"
#include "core/i_position_component.h"
#include "position_message.h"
#include "platform/auto_id.h"
#include "platform/settings.h"

namespace network {


void PositionMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GeneratePositionMessage( actor ) );
}


void PositionMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GeneratePositionMessage( actor ) );
}

PositionMessageSenderSystem::PositionMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "position" ) )
{

}

void PositionMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.position", 0.01 ) );
}

void PositionMessageSenderSystem::Update( double DeltaTime )
{
    ActorTimerMessageSenderSystem::Update( DeltaTime );
    if (!IsTime())
    {
        return;
    }
}

std::auto_ptr<PositionMessage> PositionMessageSenderSystem::GeneratePositionMessage( Actor const& actor )
{
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    if ( !positionC.IsValid() )
    {
        return std::auto_ptr<PositionMessage>();
    }
    std::auto_ptr<PositionMessage> positionMsg( new PositionMessage );
    positionMsg->mX = std::floor( positionC->GetX() * PRECISION );
    positionMsg->mY = std::floor( positionC->GetY() * PRECISION );
    //positionMsg->mOrientation=positionC->GetOrientation();
    positionMsg->mActorGUID = actor.GetGUID();
    return positionMsg;
}

} // namespace engine

