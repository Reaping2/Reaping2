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
        SetFrequency(10);
        mSendMoves.insert(platform::AutoId("player"));
        mSendMoves.insert(platform::AutoId("spider1"));
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
            if (mSendMoves.find(actor.GetId())==mSendMoves.end())
            {
                continue;
            }
            std::auto_ptr<MoveMessage> moveMessage(GenerateMoveMessage(actor));
            if (moveMessage.get()!=NULL)
            {
                mSingleMessageSender.Add(actor.GetGUID(),moveMessage);
            }
        }
        

    }

    std::auto_ptr<MoveMessage> MoveMessageSenderSystem::GenerateMoveMessage(Actor &actor)
    {
        Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
        if (!moveC.IsValid())
        {
            return std::auto_ptr<MoveMessage>();
        }
        std::auto_ptr<MoveMessage> moveMsg(new MoveMessage);
        //moveMsg->mHeading=moveC->GetHeading();
        moveMsg->mHeadingModifier=std::floor(moveC->GetHeadingModifier()*PRECISION);
        moveMsg->mSpeed=std::floor(moveC->GetSpeed()*PRECISION);
        //moveMsg->mSpeedX=moveC->GetSpeedX();
        //moveMsg->mSpeedY=moveC->GetSpeedY();
        moveMsg->mActorGUID=actor.GetGUID();
        return moveMsg;
    }

} // namespace engine

