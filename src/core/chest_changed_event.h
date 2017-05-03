#ifndef INCLUDED_ENGINE_CHEST_CHANGED_EVENT_H
#define INCLUDED_ENGINE_CHEST_CHANGED_EVENT_H

#include "platform/event.h"

namespace engine {

struct ChestChangedEvent : public platform::Event
{
    int32_t mActorGUID = -1;
    int32_t mPlayerGUID = -1;
    enum State : int32_t
    {
        StartOpen,
        Opened
    } mState = StartOpen;
    ChestChangedEvent() = default;
    ChestChangedEvent(int32_t actorGUID, int32_t playerGUID, State state)
        :mActorGUID(actorGUID), mPlayerGUID( playerGUID ), mState(state){}
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void engine::ChestChangedEvent::serialize( Archive& ar, const unsigned int version )
{
    ar& mActorGUID;
    ar& mState;
    ar& mPlayerGUID;
}

} // namespace engine

#endif//INCLUDED_ENGINE_CHEST_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "chest_changed_event" -m "int32_t-actorGUID State-state"
