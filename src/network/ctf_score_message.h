#ifndef INCLUDED_NETWORK_CTF_SCORE_H
#define INCLUDED_NETWORK_CTF_SCORE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/ctf_program_state.h"
#include "platform/export.h"

namespace network {
namespace ctf {

class CtfScoreMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( CtfScoreMessage )
    int32_t mBlueScore;
    int32_t mRedScore;
    CtfScoreMessage()
        : mBlueScore( 0 )
        , mRedScore( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void CtfScoreMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mBlueScore;
    ar& mRedScore;
}

class CtfScoreMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( CtfScoreMessageHandlerSubSystem )
    CtfScoreMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
    virtual void Update( double DeltaTime );
private:
    ::ctf::ProgramState& mCtfProgramState;
};

class CtfScoreMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE( CtfScoreMessageSenderSystem )
    CtfScoreMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    ::ctf::ProgramState& mCtfProgramState;
};

} // namespace ctf
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__ctf__CtfScoreMessage, network::ctf::CtfScoreMessage, "ctf_score" );
#endif//INCLUDED_NETWORK_CTF_SCORE_H

//command:  "classgenerator.exe" -g "message" -c "ctf_score" -m "int32_t-blueScore int32_t-redScore"
