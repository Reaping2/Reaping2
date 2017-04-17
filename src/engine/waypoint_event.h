#ifndef INCLUDED_ENGINE_WAYPOINT_EVENT_H
#define INCLUDED_ENGINE_WAYPOINT_EVENT_H

#include "platform/i_platform.h"


namespace engine {

struct WaypointChangedEvent : public platform::Event
{
    std::string mWaypointName;
    int32_t mWaypointGUID = -1;
    int32_t mPlayerGUID = -1;
    enum State : int32_t
    {
        Lit = 0,
        Choose,
        Selected
    } mState = Lit;
    WaypointChangedEvent() = default;
    WaypointChangedEvent(std::string waypointName, int32_t waypointGUID, int32_t playerGUID, State state)
        :mWaypointName(waypointName),mWaypointGUID(waypointGUID), mPlayerGUID(playerGUID),mState(state){}
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );

};

template<class Archive>
void engine::WaypointChangedEvent::serialize( Archive& ar, const unsigned int version )
{
    ar& mWaypointName;
    ar& mWaypointGUID;
    ar& mPlayerGUID;
    ar& mState;
}

} // namespace engine

#endif//INCLUDED_ENGINE_WAYPOINT_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "waypoint_event" -n "engine" -m "std::string-waypointName int32_t-actorGUID State-state"
