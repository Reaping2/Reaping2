#ifndef INCLUDED_NETWORK_DATA_CHECKSUM_H
#define INCLUDED_NETWORK_DATA_CHECKSUM_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"

namespace network {

class DataChecksumMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(DataChecksumMessage)
    std::string mDatasource;
    int32_t mChecksum;
    DataChecksumMessage()
        : mDatasource("")
        , mChecksum(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void DataChecksumMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mDatasource;
    ar & mChecksum;
}

class DataChecksumMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(DataChecksumMessageHandlerSubSystem)
    DataChecksumMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class DataChecksumMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(DataChecksumMessageSenderSystem)
    DataChecksumMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network
REAPING2_CLASS_EXPORT_KEY2( network__DataChecksumMessage, network::DataChecksumMessage, "data_checksum_message" )
#endif//INCLUDED_NETWORK_DATA_CHECKSUM_H
//command:  "./classgenerator" -g "message" -c "data_checksum" -m "string-datasource int32_t-checksum"
