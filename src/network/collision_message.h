#ifndef INCLUDED_NETWORK_COLLISION_H
#define INCLUDED_NETWORK_COLLISION_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/collision_class.h"
#include <boost/serialization/export.hpp>

namespace network {

class CollisionMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(CollisionMessage)
    int32_t mActorGUID;
    CollisionClass::Type mCollisionClass;
    double mRadius;
    CollisionMessage()
        : mActorGUID(0)
        , mCollisionClass(CollisionClass::No_Collision)
        , mRadius(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void CollisionMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mCollisionClass;
    ar & mRadius;
}

class CollisionMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<CollisionMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE(CollisionMessageHandlerSubSystem)
    CollisionMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    virtual bool ProcessPending(Message const& message);
};
class CollisionMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(CollisionMessageSenderSystem)
    CollisionMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<CollisionMessage> GenerateCollisionMessage(Actor &actor);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::CollisionMessage,"collision");
#endif//INCLUDED_NETWORK_COLLISION_H

//command:  "classgenerator.exe" -g "message" -c "collision" -m "int32_t-actorGUID CollisionClass::Type-collisionClass double-radius"
