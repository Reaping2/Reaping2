#ifndef INCLUDED_NETWORK_KILL_SCORE_H
#define INCLUDED_NETWORK_KILL_SCORE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/kill_score_event.h"
#include "platform/export.h"

namespace network {

class KillScoreMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( KillScoreMessage )
    int32_t mKillerGUID;
    int32_t mDeadGUID;
    KillScoreMessage()
        : mKillerGUID( -1 )
        , mDeadGUID( -1 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void KillScoreMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mKillerGUID;
    ar& mDeadGUID;
}

class KillScoreMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( KillScoreMessageHandlerSubSystem )
    KillScoreMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
    virtual void Update( double DeltaTime );
};

class KillScoreMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnKillScore;
    void OnKillScore( engine::KillScoreEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE( KillScoreMessageSenderSystem )
    KillScoreMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__KillScoreMessage, network::KillScoreMessage, "kill_score" );
#endif//INCLUDED_NETWORK_KILL_SCORE_H

//command:  "classgenerator.exe" -g "message" -c "kill_score" -m "int32_t-killerGUID int32_t-deathGUID" -e "engine-killScore"
