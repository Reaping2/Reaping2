#include "platform/i_platform.h"
#include "network/rotate_message.h"
#include "core/i_rotate_component.h"

namespace network {

bool RotateMessage::operator==( RotateMessage const& other )
{
    return mActorGUID == other.mActorGUID
        && mSpeed == other.mSpeed
        && mRotating == other.mRotating;
}
RotateMessageSenderSystem::RotateMessageSenderSystem()
    : MessageSenderSystem()
{
}


void RotateMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    SetFrequency( 10 );
    if (mProgramState.mMode == ProgramState::Server)
    {
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "rusty_reaper_alt_projectile" ) ) );
    }
}


void RotateMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
    mActorFrequencyTimerHolder.Update( DeltaTime );
    if (mProgramState.mMode == ProgramState::Server)
    {
        auto mSendPositions = mActorFrequencyTimerHolder.GetActorIds();
        if (mSendPositions.empty())
        {
            return;
        }

        for (ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it)
        {
            Actor& actor = **it;
            if (mSendPositions.find( actor.GetId() ) == mSendPositions.end())
            {
                continue;
            }
            std::auto_ptr<RotateMessage> rotateMessage( GenerateRotateMessage( actor ) );
            if (rotateMessage.get() != NULL)
            {
                mSingleMessageSender.Add( actor.GetGUID(), rotateMessage );
            }

        }
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

