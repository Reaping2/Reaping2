#ifndef INCLUDED_NETWORK_MAP_START_H
#define INCLUDED_NETWORK_MAP_START_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "core/map_start_event.h"

namespace network {

class MapStartMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(MapStartMessage)
    core::MapStartEvent::State mState;
    MapStartMessage()
        : mState( core::MapStartEvent::Ready)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( MapStartMessage const& other );
};

template<class Archive>
void MapStartMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mState;
}

class MapStartMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(MapStartMessageHandlerSubSystem)
    MapStartMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class MapStartMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnMapStart;
    void OnMapStart(core::MapStartEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(MapStartMessageSenderSystem)
    MapStartMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_MAP_START_H

REAPING2_CLASS_EXPORT_KEY2( network__MapStartMessage, network::MapStartMessage, "map_start" );

//command:  "classgenerator.exe" -g "message" -c "map_start" -m "MapStartEvent::State-state" -e "core-mapStart"
