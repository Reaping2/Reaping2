#include "actor_frequency_timer.h"
namespace network {

ActorFrequencyTimer::ActorFrequencyTimer( double frequency, int32_t actorId )
    : FrequencyTimer()
    , mActorId( actorId )
{
    SetFrequency( frequency );
}

int32_t ActorFrequencyTimer::GetActorId() const
{
    return mActorId;
}

void ActorFrequencyTimerHolder::Add( ActorFrequencyTimer const& actorFrequencyTimer )
{
    mActorFrequencyTimers.push_back( actorFrequencyTimer );
}

void ActorFrequencyTimerHolder::Update( double DeltaTime )
{
    for( auto&& freqTimer : mActorFrequencyTimers )
    {
        freqTimer.Update( DeltaTime );
    }
}

ActorFrequencyTimerHolder::ActorIds_t ActorFrequencyTimerHolder::GetActorIds() const
{
    ActorIds_t actorIds;
    for( auto&& freqTimer : mActorFrequencyTimers )
    {
        if (freqTimer.IsTime() )
        {
            actorIds.insert( freqTimer.GetActorId() );
        }
    }
    return actorIds;
}

} // namespace network

