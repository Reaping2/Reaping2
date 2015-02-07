#include "network/position_message_sender_system.h"
#include "core/i_position_component.h"
#include "position_message.h"
namespace network {


    PositionMessageSenderSystem::PositionMessageSenderSystem()
        : MessageSenderSystem()
    {

    }

    void PositionMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        SetFrequency(50);
    }

    void PositionMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
        if (IsClient())
        {
            //TODO: might need optimization
            Opt<Actor> actor=mScene.GetActor(mProgramState.mControlledActorGUID);
            if (actor.IsValid())
            {
                Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
                if (positionC.IsValid())
                {
                    std::auto_ptr<PositionMessage> positionMsg(new PositionMessage);
                    positionMsg->mX=double(positionC->GetX());
                    positionMsg->mY=double(positionC->GetY());
                    positionMsg->mOrientation=positionC->GetOrientation();
                    positionMsg->mActorGUID=actor->GetGUID();
                    mMessageHolder.AddOutgoingMessage(positionMsg);
                }
            }
        }
        else
        {
            for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
            {
                Actor& actor=**it;
                Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
                if (!positionC.IsValid())
                {
                    continue;
                }
                std::auto_ptr<PositionMessage> positionMsg(new PositionMessage);
                positionMsg->mX=double(positionC->GetX());
                positionMsg->mY=double(positionC->GetY());
                positionMsg->mOrientation=positionC->GetOrientation();
                positionMsg->mActorGUID=actor.GetGUID();
                mMessageHolder.AddOutgoingMessage(positionMsg);
            }
        }

    }

} // namespace engine

