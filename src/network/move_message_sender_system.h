#ifndef INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H

#include "message_sender_system.h"
#include "move_message.h"
#include <set>
#include "single_message_sender.h"

namespace network {



class MoveMessageSenderSystem: public ActorTimerMessageSenderSystem<MoveMessage>
{
    typedef std::set<int32_t> SendMovess_t;
    SendMovess_t mSendMoves;
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE( MoveMessageSenderSystem )
    MoveMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    static std::auto_ptr<MoveMessage> GenerateMoveMessage( Actor& actor );

};

} // namespace network

#endif//INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H
