#ifndef INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
#define INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H

#include "network/message.h"
namespace network {

    class CreateActorMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(CreateActorMessage)

        int32_t mActorId;
        int32_t mActorGUID;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void CreateActorMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorId;
        ar & mActorGUID;
    }

} // namespace network
#endif//INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
