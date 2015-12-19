#ifndef INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H
#define INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H

#include "network/message.h"
#include "core/program_state.h"
namespace network {

    class LifecycleMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(LifecycleMessage)
        enum State
        {
            Start=0,
            Restart,
            Exit
        };
        State mState;
        std::string mMode;
        std::string mLevel;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void LifecycleMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mState;
        ar & mMode;
        ar & mLevel;
    }

} // namespace network
#endif//INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H
