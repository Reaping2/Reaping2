#ifndef INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H

#include "engine/system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"
#include <set>
#include "platform/frequency_timer.h"
#include "single_message_sender.h"
#include "engine/engine.h"
#include "actor_frequency_timer.h"
#include "message_timer_system.h"
using core::ProgramState;

namespace network {

class MessageSenderSystem: public engine::System
{
protected:
    FrequencyTimer mFrequencyTimer;
    MessageHolder& mMessageHolder;
    Scene& mScene;
    ProgramState& mProgramState;
    bool mIsClient;
    bool IsClient() const;
    bool mIsServer;
    bool IsServer() const;
    bool IsTime() const;
    void SetFrequency( double frequency );
public:
    MessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

template<class MESSAGE>
class ActorTimerMessageSenderSystem : public MessageSenderSystem
{
    void AddMessage( Actor& actor, ActorFrequencyTimerHolder::ActorIds_t& sendUniqueMessages, ActorFrequencyTimerHolder::ActorIds_t& sendMandatoryMessages );
public:
    ActorTimerMessageSenderSystem( int32_t messageJsonId );
    virtual void Init();
    virtual void Update( double DeltaTime );
protected:
    // should put the message into mUniqueMessageSender
    virtual void AddUniqueMessage( Actor& actor ) = 0;
    // should put the message into mMessageHolder
    virtual void AddMandatoryMessage( Actor& actor ) = 0;
    AutoActorGUIDUniqueMessageSender<MESSAGE> mUniqueMessageSender;
    int32_t mMessageJsonId;
    Opt<MessageTimerSystem> mMessageTimerSystem;

};

template<class MESSAGE>
void ActorTimerMessageSenderSystem<MESSAGE>::AddMessage( Actor& actor, ActorFrequencyTimerHolder::ActorIds_t& sendUniqueMessages, ActorFrequencyTimerHolder::ActorIds_t& sendMandatoryMessages )
{
    if (sendUniqueMessages.find( actor.GetId() ) != sendUniqueMessages.end())
    {
        AddUniqueMessage( actor );
    }
    if (sendMandatoryMessages.find( actor.GetId() ) != sendMandatoryMessages.end())
    {
        AddMandatoryMessage( actor );
    }
}

template<class MESSAGE>
void ActorTimerMessageSenderSystem<MESSAGE>::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    if (!IsTime())
    {
        return;
    }
    auto messageTimer(mMessageTimerSystem->GetMessageTimer( mMessageJsonId ));
    if (messageTimer.IsValid())
    {
        auto sendUniqueMessages = messageTimer->GetUnique().GetActorIds();
        auto sendMandatoryMessages = messageTimer->GetMandatory().GetActorIds();
        if (mIsServer)
        {
            for (auto actor : mScene.GetActors())
            {
                AddMessage( *actor, sendUniqueMessages, sendMandatoryMessages );
            }
        }
        else if (mIsClient)
        {
            auto player( mScene.GetActor( mProgramState.mControlledActorGUID ) );
            if (player.IsValid())
            {
                AddMessage( *player, sendUniqueMessages, sendMandatoryMessages );
            }
        }
    }
}


template<class MESSAGE>
ActorTimerMessageSenderSystem<MESSAGE>::ActorTimerMessageSenderSystem( int32_t messageJsonId )
    : MessageSenderSystem()
    , mMessageJsonId( messageJsonId )
{

}

template<class MESSAGE>
void ActorTimerMessageSenderSystem<MESSAGE>::Init()
{
    MessageSenderSystem::Init();
    mMessageTimerSystem = engine::Engine::Get().GetSystem<MessageTimerSystem>();
}

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H
