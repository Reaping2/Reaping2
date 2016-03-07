#ifndef INCLUDED_NETWORK_SET_OWNERSHIP_H
#define INCLUDED_NETWORK_SET_OWNERSHIP_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/soldier_created_event.h"
#include "platform/export.h"

namespace network {

class SetOwnershipMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( SetOwnershipMessage )
    int32_t mActorGUID;
    int32_t mClientId;
    SetOwnershipMessage()
        : mActorGUID( 0 )
        , mClientId( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void SetOwnershipMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mClientId;
}

class SetOwnershipMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<SetOwnershipMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE( SetOwnershipMessageHandlerSubSystem )
    SetOwnershipMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    virtual bool ProcessPending( Message const& message );
};

class SetOwnershipMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnSoldierCreated;
    void OnSoldierCreated( engine::SoldierCreatedEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE( SetOwnershipMessageSenderSystem )
    SetOwnershipMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__SetOwnershipMessage, network::SetOwnershipMessage, "set_ownership" );
#endif//INCLUDED_NETWORK_SET_OWNERSHIP_H

