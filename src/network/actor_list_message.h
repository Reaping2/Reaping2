#ifndef INCLUDED_NETWORK_ACTOR_LIST_H
#define INCLUDED_NETWORK_ACTOR_LIST_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"

namespace network {

class ActorListMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( ActorListMessage )
    std::string mActorList;
    int32_t mClientId;
    ActorListMessage( ActorList_t* actorList = nullptr );
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ActorListMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorList;
    ar& mClientId;
}

class ActorListMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( ActorListMessageHandlerSubSystem )
    ActorListMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
    virtual void Update( double DeltaTime );
};

class ActorListMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE( ActorListMessageSenderSystem )
    ActorListMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__ActorListMessage, network::ActorListMessage, "actor_list" );
#endif//INCLUDED_NETWORK_ACTOR_LIST_H

//command:  "classgenerator.exe" -g "message" -c "actor_list" -m "std::string-actorList int32_t-clientId"
