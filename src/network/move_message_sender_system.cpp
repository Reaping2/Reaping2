#include "network/move_message_sender_system.h"
#include "core/i_move_component.h"
#include "move_message.h"
#include "platform/settings.h"

namespace network {


void MoveMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateMoveMessage( actor ) );
}


void MoveMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateMoveMessage( actor ) );
}

MoveMessageSenderSystem::MoveMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "move" ) )
{
}

void MoveMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.move", 0.01 ) );
}

void MoveMessageSenderSystem::Update(double DeltaTime)
{
    ActorTimerMessageSenderSystem::Update(DeltaTime);
    if (!IsTime())
    {
        return;
    }
}

std::auto_ptr<MoveMessage> MoveMessageSenderSystem::GenerateMoveMessage( Actor& actor )
{
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( !moveC.IsValid() )
    {
        return std::auto_ptr<MoveMessage>();
    }
    std::auto_ptr<MoveMessage> moveMsg( new MoveMessage );
    moveMsg->mHeadingModifier = std::floor( moveC->GetHeadingModifier() * PRECISION );
    moveMsg->mSpeed = std::floor( moveC->GetSpeed().mBase.Get() * PRECISION );
    moveMsg->mPercent = std::floor( moveC->GetSpeed().mPercent.Get() * PRECISION );
    moveMsg->mFlat = std::floor( moveC->GetSpeed().mFlat.Get() * PRECISION );
    moveMsg->mMoving = moveC->GetMoving();
    moveMsg->mActorGUID = actor.GetGUID();
    moveMsg->mRooted = moveC->IsRooted();
    return moveMsg;
}

} // namespace engine

