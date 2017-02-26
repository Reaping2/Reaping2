#ifndef INCLUDED_NETWORK_MAP_LOAD_H
#define INCLUDED_NETWORK_MAP_LOAD_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "core/map_load_event.h"

namespace network {

class MapLoadMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(MapLoadMessage)
    std::string mMapName;
    MapLoadMessage()
        : mMapName()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( MapLoadMessage const& other );
};

template<class Archive>
void MapLoadMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mMapName;
}

class MapLoadMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(MapLoadMessageHandlerSubSystem)
    MapLoadMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class MapLoadMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnMapLoad;
    void OnMapLoad(core::MapLoadEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(MapLoadMessageSenderSystem)
    MapLoadMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_MAP_LOAD_H

REAPING2_CLASS_EXPORT_KEY2( network__MapLoadMessage, network::MapLoadMessage, "map_load" );


//command:  "classgenerator.exe" -g "message" -c "map_load" -m "std::string-mapName" -e "core-mapLoad"
