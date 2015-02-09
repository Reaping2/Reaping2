#ifndef INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
#define INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H

#include "network/message.h"
#include "core/actor_event.h"
namespace network {

    class CreateActorMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(CreateActorMessage)

        int32_t mActorId;
        int32_t mActorGUID;
        int32_t mParentGUID; //for shots atm, //TODO: there will be an engine upgrade for this
        ActorEvent::State mState;

        CreateActorMessage():mActorId(-1), mActorGUID(-1),mParentGUID(-1){}

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void CreateActorMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorId;
        ar & mActorGUID;
        ar & mParentGUID;
        ar & mState;
    }

} // namespace network
#endif//INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
