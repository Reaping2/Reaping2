#include "network/position_message_sender_system.h"
#include "core/i_position_component.h"
#include "position_message.h"
#include "platform/auto_id.h"
namespace network {


    PositionMessageSenderSystem::PositionMessageSenderSystem()
        : MessageSenderSystem()
    {

    }

    void PositionMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        SetFrequency(10);
        mSendPositions.insert(platform::AutoId("player"));
    }

    void PositionMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
        for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
        {
            Actor& actor=**it;
            if (mSendPositions.find(actor.GetId())==mSendPositions.end())
            {
                continue;
            }
            mMessageHolder.AddOutgoingMessage(GeneratePositionMessage(actor));
            L1("Sending position for actor: %d\n",actor.GetId());
        }

    }

    std::auto_ptr<PositionMessage> PositionMessageSenderSystem::GeneratePositionMessage(Actor const& actor)
    {
        Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
        if (!positionC.IsValid())
        {
            return std::auto_ptr<PositionMessage>();
        }
        std::auto_ptr<PositionMessage> positionMsg(new PositionMessage);
        positionMsg->mX=double(positionC->GetX());
        positionMsg->mY=double(positionC->GetY());
        positionMsg->mOrientation=positionC->GetOrientation();
        positionMsg->mActorGUID=actor.GetGUID();
        return positionMsg;
    }

} // namespace engine

