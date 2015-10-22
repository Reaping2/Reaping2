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
//         mSendPositions.insert(platform::AutoId("player"));
//         mSendPositions.insert(platform::AutoId("spider1"));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(600.0,platform::AutoId("spider1")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(600.0,platform::AutoId("spider2")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(300.0,platform::AutoId("spider1target")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(300.0,platform::AutoId("spider2target")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(300.0,platform::AutoId("player")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(300.0,platform::AutoId("ctf_player")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(100.0,platform::AutoId("grenade_projectile")));
        mActorFrequencyTimerHolder.Add(ActorFrequencyTimer(100.0,platform::AutoId("rocket_launcher_target_projectile")));

    }

    void PositionMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
        mActorFrequencyTimerHolder.Update(DeltaTime);
        if (!IsTime())
        {
            return;
        }
        mSendPositions=mActorFrequencyTimerHolder.GetActorIds();
        if (mSendPositions.empty())
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
            std::auto_ptr<PositionMessage> positionMessage(GeneratePositionMessage(actor));
            if (positionMessage.get()!=NULL)
            {
                mSingleMessageSender.Add(actor.GetGUID(),positionMessage);
            }

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
        positionMsg->mX=std::floor(positionC->GetX()*PRECISION);
        positionMsg->mY=std::floor(positionC->GetY()*PRECISION);
        //positionMsg->mOrientation=positionC->GetOrientation();
        positionMsg->mActorGUID=actor.GetGUID();
        return positionMsg;
    }

} // namespace engine

