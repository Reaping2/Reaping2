#ifndef INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H

#include "engine/system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"
#include <set>
#include "platform/frequency_timer.h"
using core::ProgramState;

namespace network {

class ActorFrequencyTimer : public FrequencyTimer
{
    int32_t mActorId;
public:
    ActorFrequencyTimer(double frequency, int32_t actorId);
    int32_t GetActorId() const;
};

class ActorFrequencyTimerHolder
{
    typedef std::list<ActorFrequencyTimer> ActorFrequencyTimers_t;
    ActorFrequencyTimers_t mActorFrequencyTimers;
    typedef std::set<int32_t> ActorIds_t;
public:
    void Add(ActorFrequencyTimer const& actorFrequencyTimer);
    void Update( double DeltaTime );
    ActorIds_t GetActorIds();
};

class MessageSenderSystem: public engine::System
{
protected:
    FrequencyTimer mFrequencyTimer;
    MessageHolder& mMessageHolder;
    Scene& mScene;
    ProgramState& mProgramState;
    bool mIsClient;
    bool IsClient();
    bool mIsServer;
    bool IsServer();
    bool IsTime();
    void SetFrequency( double frequency );
public:
    MessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H
