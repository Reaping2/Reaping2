#ifndef INCLUDED_NETWORK_SHOW_TEXT_MESSAGE_H
#define INCLUDED_NETWORK_SHOW_TEXT_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/show_text_event.h"
#include <boost/serialization/export.hpp>

namespace network {

class ShowTextMessageMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(ShowTextMessageMessage)
    double mSeconds;
    std::string mText;
    ShowTextMessageMessage()
        : mSeconds(0.0)
        , mText("")
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ShowTextMessageMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mSeconds;
    ar & mText;
}

class ShowTextMessageMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(ShowTextMessageMessageHandlerSubSystem)
    ShowTextMessageMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class ShowTextMessageMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnShowText;
    void OnShowText(engine::ShowTextEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(ShowTextMessageMessageSenderSystem)
    ShowTextMessageMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::ShowTextMessageMessage,"show_text_message");
#endif//INCLUDED_NETWORK_SHOW_TEXT_MESSAGE_H

//command:  "classgenerator.exe" -g "message" -c "show_text_message" -m "double-seconds std::string-text" -e "engine-showText"
