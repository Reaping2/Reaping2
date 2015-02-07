#ifndef INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_SENDER_SYSTEM_H

#include "engine/system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"
using core::ProgramState;

namespace network {
    class FrequencyTimer
    {
        double mElapsedTime;
        double mFrequency;
        bool mIsTime;
    public:
        FrequencyTimer();
        bool Update( double DeltaTime );
        bool IsTime();
        void SetFrequency( double frequency );
        void Reset();
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
