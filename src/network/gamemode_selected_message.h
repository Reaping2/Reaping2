#ifndef INCLUDED_NETWORK_GAMEMODE_SELECTED_MESSAGE_H
#define INCLUDED_NETWORK_GAMEMODE_SELECTED_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/gamemode_selected_event.h"

namespace network {

class GamemodeSelectedMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(GamemodeSelectedMessage)
    std::string mGameMode;
    int32_t mOriginator;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void GamemodeSelectedMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mGameMode;
    ar & mOriginator;
}

class GamemodeSelectedMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(GamemodeSelectedMessageHandlerSubSystem)
    GamemodeSelectedMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class GamemodeSelectedMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnGamemodeSelectedEvent;
    void OnGamemodeSelectedEvent(core::GamemodeSelectedEvent const & evt);
public:
    DEFINE_SYSTEM_BASE(GamemodeSelectedMessageSenderSystem)
    GamemodeSelectedMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_GAMEMODE_SELECTED_MESSAGE_H

//command:  "./classgenerator" -g "message" -c "gamemode_selected_message"
