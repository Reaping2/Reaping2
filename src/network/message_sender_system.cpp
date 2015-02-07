#include "network/message_sender_system.h"
namespace network {

    FrequencyTimer::FrequencyTimer()
        : mFrequency(0.0)
        , mElapsedTime(0.0)
        , mIsTime(true)
    {
    }

    bool FrequencyTimer::Update(double DeltaTime)
    {
        mIsTime=false;
        mElapsedTime+=DeltaTime*1000;
        if(mElapsedTime>=mFrequency)
        {
            mIsTime=true;
            mElapsedTime=(int(mElapsedTime)%int(mFrequency));
        }
        return mIsTime;
    }

    bool FrequencyTimer::IsTime()
    {
        return mIsTime;
    }

    void FrequencyTimer::SetFrequency(double frequency)
    {
        mFrequency=frequency;
    }

    void FrequencyTimer::Reset()
    {
        mElapsedTime=0.0;
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

} // namespace engine

