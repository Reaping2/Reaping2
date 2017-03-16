#ifndef INCLUDED_NETWORK_WAYPOINT_H
#define INCLUDED_NETWORK_WAYPOINT_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "engine/waypoint_event.h"

namespace network {

class WaypointMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(WaypointMessage)
    engine::WaypointChangedEvent mEvent;
    WaypointMessage()
        : mEvent()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void WaypointMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mEvent;
}

class WaypointMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(WaypointMessageHandlerSubSystem)
    WaypointMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class WaypointMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnWaypointChanged;
    void OnWaypointChanged(engine::WaypointChangedEvent const& Evt);
    void CreateMessage( engine::WaypointChangedEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE(WaypointMessageSenderSystem)
    WaypointMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_WAYPOINT_H

REAPING2_CLASS_EXPORT_KEY2( network__WaypointMessage, network::WaypointMessage, "waypoint" );

//command:  "classgenerator.exe" -g "message" -c "waypoint" -m "WaypointChangedEvent-event" -e "engine-waypointChanged"
