#ifndef INCLUDED_NETWORK_ACTOR_FREQUENCY_TIMER_H
#define INCLUDED_NETWORK_ACTOR_FREQUENCY_TIMER_H

#include <set>
#include "platform/frequency_timer.h"
#include <vector>

namespace network {

class ActorFrequencyTimer : public platform::FrequencyTimer
{
    int32_t mActorId;
public:
    ActorFrequencyTimer( double frequency, int32_t actorId );
    int32_t GetActorId() const;
};

class ActorFrequencyTimerHolder
{
    typedef std::vector<ActorFrequencyTimer> ActorFrequencyTimers_t;
    ActorFrequencyTimers_t mActorFrequencyTimers;
public:
    void Add( ActorFrequencyTimer const& actorFrequencyTimer );
    void Update( double DeltaTime );
    typedef std::set<int32_t> ActorIds_t;
    ActorIds_t GetActorIds() const;
};


} // namespace network

#endif//INCLUDED_NETWORK_ACTOR_FREQUENCY_TIMER_H
