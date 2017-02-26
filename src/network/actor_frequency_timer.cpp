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
    for( ActorFrequencyTimers_t::iterator it = mActorFrequencyTimers.begin(), e = mActorFrequencyTimers.end(); it != e; ++it )
    {
        it->Update( DeltaTime );
    }
}

ActorFrequencyTimerHolder::ActorIds_t ActorFrequencyTimerHolder::GetActorIds() const
{
    ActorIds_t actorIds;
    for( auto&& timer : mActorFrequencyTimers )
    {
        if ( timer.IsTime() )
        {
            actorIds.insert( timer.GetActorId() );
        }
    }
    return actorIds;
}

} // namespace network

