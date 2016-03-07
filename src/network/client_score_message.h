#ifndef INCLUDED_NETWORK_CLIENT_SCORE_H
#define INCLUDED_NETWORK_CLIENT_SCORE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/client_score_event.h"
#include "platform/export.h"

namespace network {

class ClientScoreMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( ClientScoreMessage )
    int32_t mClientID;
    int32_t mScore;
    ClientScoreMessage()
        : mClientID( -1 )
        , mScore( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ClientScoreMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mClientID;
    ar& mScore;
}

class ClientScoreMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( ClientScoreMessageHandlerSubSystem )
    ClientScoreMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
    virtual void Update( double DeltaTime );
};

class ClientScoreMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnClientScore;
    void OnClientScore( engine::ClientScoreEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE( ClientScoreMessageSenderSystem )
    ClientScoreMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__ClientScoreMessage, network::ClientScoreMessage, "client_score" );
#endif//INCLUDED_NETWORK_CLIENT_SCORE_H

//command:  "classgenerator.exe" -g "message" -c "client_score" -m "int32_t-clientID int32_t-score" -e "engine-clientScore"
