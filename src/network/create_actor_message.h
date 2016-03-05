#ifndef INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
#define INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H

#include "network/message.h"
#include "core/actor_event.h"
#include "platform/export.h"
namespace network {

    class CreateActorMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(CreateActorMessage)

        int32_t mActorGUID;
        ActorEvent::State mState;
        std::string mActor;
        CreateActorMessage(Opt<Actor> actor=Opt<Actor>(NULL));

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void CreateActorMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        ar & mState;
        ar & mActor;
    }

} // namespace network

REAPING2_CLASS_EXPORT_KEY2(network__CreateActorMessage, network::CreateActorMessage,"create_actor");
#endif//INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_H
