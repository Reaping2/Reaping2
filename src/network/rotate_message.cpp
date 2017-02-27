#include "platform/i_platform.h"
#include "network/rotate_message.h"
#include "core/i_rotate_component.h"
#include "platform/settings.h"

namespace network {

bool RotateMessage::operator==( RotateMessage const& other )
{
    return mActorGUID == other.mActorGUID
        && mSpeed == other.mSpeed
        && mRotating == other.mRotating;
}

void RotateMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateRotateMessage( actor ) );
}


void RotateMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateRotateMessage( actor ) );
}

RotateMessageSenderSystem::RotateMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "rotate" ) )
{
}


void RotateMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.rotate", 0.01 ) );
}


void RotateMessageSenderSystem::Update(double DeltaTime)
{
    ActorTimerMessageSenderSystem::Update(DeltaTime);
    if (!IsTime())
    {
        return;
    }
}

RotateMessageHandlerSubSystem::RotateMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void RotateMessageHandlerSubSystem::Init()
{
}

void RotateMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void RotateMessageHandlerSubSystem::Execute(Message const& message)
{
    RotateMessage const& msg=static_cast<RotateMessage const&>(message);
    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
    if (!actor.IsValid())
    {
        L1("cannot find actor with GUID: (%s) %d \n",__FUNCTION__,msg.mActorGUID );
        return;
    }
    Opt<IRotateComponent> rotateC( actor->Get<IRotateComponent>() );
    if (!rotateC.IsValid())
    {
        L1( "rotate is called on an actor that has no rotate_component \n" );
        return;
    }
    rotateC->SetSpeed( msg.mSpeed );
    rotateC->SetRotating( msg.mRotating );
}

std::auto_ptr<RotateMessage> RotateMessageSenderSystem::GenerateRotateMessage(Actor &actor)
{
    Opt<IRotateComponent> rotateC = actor.Get<IRotateComponent>();
    if (!rotateC.IsValid())
    {
        return std::auto_ptr<RotateMessage>();
    }
    std::auto_ptr<RotateMessage> rotateMsg(new RotateMessage);
    rotateMsg->mActorGUID=actor.GetGUID();
    rotateMsg->mSpeed=rotateC->GetSpeed();
    rotateMsg->mRotating=rotateC->IsRotating();
    return rotateMsg;
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__RotateMessage, network::RotateMessage );

