#include "network/message_sender_system.h"
namespace network {

    ActorFrequencyTimer::ActorFrequencyTimer(double frequency, int32_t actorId)
        : FrequencyTimer()
        , mActorId(actorId)
    {
        SetFrequency(frequency);
    }

    int32_t ActorFrequencyTimer::GetActorId() const
    {
        return mActorId;
    }

    MessageSenderSystem::MessageSenderSystem()
        : mFrequencyTimer()
        , mMessageHolder(MessageHolder::Get())
        , mScene(Scene::Get())
        , mProgramState(ProgramState::Get())
    {
        mIsClient=mProgramState.mMode==ProgramState::Client;
        mIsServer=mProgramState.mMode==ProgramState::Server;
    }

    void MessageSenderSystem::Update(double DeltaTime)
    {
        mFrequencyTimer.Update(DeltaTime);
    }

    bool MessageSenderSystem::IsTime()
    {
        return mFrequencyTimer.IsTime();
    }

    void MessageSenderSystem::SetFrequency(double frequency)
    {
        mFrequencyTimer.SetFrequency(frequency);
    }

    void MessageSenderSystem::Init()
    {
        mFrequencyTimer.Reset();
    }

    bool MessageSenderSystem::IsClient()
    {
        return mIsClient;
    }

    bool MessageSenderSystem::IsServer()
    {
        return mIsServer;
    }





    void ActorFrequencyTimerHolder::Add(ActorFrequencyTimer const& actorFrequencyTimer)
    {
        mActorFrequencyTimers.push_back(actorFrequencyTimer);
    }

    void ActorFrequencyTimerHolder::Update(double DeltaTime)
    {
        for( ActorFrequencyTimers_t::iterator it = mActorFrequencyTimers.begin(), e = mActorFrequencyTimers.end(); it != e; ++it )
        {
            it->Update(DeltaTime);
        }
    }

    ActorFrequencyTimerHolder::ActorIds_t ActorFrequencyTimerHolder::GetActorIds()
    {
        ActorIds_t actorIds;
        for( ActorFrequencyTimers_t::iterator it = mActorFrequencyTimers.begin(), e = mActorFrequencyTimers.end(); it != e; ++it )
        {
            if (it->IsTime())
            {
                actorIds.insert(it->GetActorId());
            }
        }
        return actorIds;
    }

} // namespace network

