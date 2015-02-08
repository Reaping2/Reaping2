#include "network/move_message_sender_system.h"
#include "core/i_move_component.h"
#include "move_message.h"
namespace network {


    MoveMessageSenderSystem::MoveMessageSenderSystem()
        : MessageSenderSystem()
    {

    }

    void MoveMessageSenderSystem::Init()
    {
        MessageSenderSystem::Init();
        SetFrequency(50);
    }

    void MoveMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
       
        for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
        {
            Actor& actor=**it;
            Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
            if (!moveC.IsValid())
            {
                continue;
            }
            if (moveC.IsValid())
            {
                std::auto_ptr<MoveMessage> moveMsg(new MoveMessage);
                moveMsg->mHeading=moveC->GetHeading();
                moveMsg->mSpeed=moveC->GetSpeed();
                moveMsg->mSpeedX=moveC->GetSpeedX();
                moveMsg->mSpeedY=moveC->GetSpeedY();
                moveMsg->mActorGUID=actor.GetGUID();

                mMessageHolder.AddOutgoingMessage(moveMsg);
            }
        }
        

    }

} // namespace engine

