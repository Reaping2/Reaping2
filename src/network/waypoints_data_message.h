#ifndef INCLUDED_NETWORK_WAYPOINTS_DATA_H
#define INCLUDED_NETWORK_WAYPOINTS_DATA_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "engine/waypoint_system.h"

namespace network {

class WaypointsDataMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(WaypointsDataMessage)
    std::string mWaypointsData;
    int32_t mClientId;
    WaypointsDataMessage( engine::WaypointsData * waypointsData = nullptr, int32_t clientId = -1 );
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( WaypointsDataMessage const& other );
};

template<class Archive>
void WaypointsDataMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mWaypointsData;
    ar& mClientId;
}

class WaypointsDataMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(WaypointsDataMessageHandlerSubSystem)
    WaypointsDataMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class WaypointsDataMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(WaypointsDataMessageSenderSystem)
    WaypointsDataMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_WAYPOINTS_DATA_H

REAPING2_CLASS_EXPORT_KEY2( network__WaypointsDataMessage, network::WaypointsDataMessage, "waypoints_data" );

//command:  "classgenerator.exe" -g "message" -c "waypoints_data" -m "std::string-waypointsData int32_t-clientId"
