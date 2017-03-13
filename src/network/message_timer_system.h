#ifndef INCLUDED_NETWORK_MESSAGE_TIMER_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_TIMER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "platform/i_platform.h"
#include "actor_frequency_timer.h"
#include <map>

namespace network {

class MessageTimer
{
public:
    ActorFrequencyTimerHolder const& GetUnique() const;
    ActorFrequencyTimerHolder const& GetMandatory() const;    
    void Load( Json::Value const& setters );
    void Update( double DeltaTime );
private:
    int32_t mId = -1;
    ActorFrequencyTimerHolder mUniqueFrequencyTimerHolder;
    ActorFrequencyTimerHolder mMandatoryFrequencyTimerHolder;
};

class MessageTimerSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(MessageTimerSystem)
    MessageTimerSystem();
    Opt<MessageTimer> GetMessageTimer( int32_t mMessageJsonId );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    typedef std::map<int32_t, MessageTimer> MessageTimers_t;
    MessageTimers_t mMessageTimers;
};

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_TIMER_SYSTEM_H


//command:  "classgenerator.exe" -g "system" -c "message_timer_system" -n "network"
