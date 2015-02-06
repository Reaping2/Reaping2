#ifndef INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_H
#define INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_H

#include "network/message.h"
namespace network {

    class SetOwnershipMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(SetOwnershipMessage)

        int32_t mActorGUID;
        int32_t mClientId;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void SetOwnershipMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        ar & mClientId;
    }

} // namespace network
#endif//INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_H
