#ifndef INCLUDED_NETWORK_POSITION_MESSAGE_H
#define INCLUDED_NETWORK_POSITION_MESSAGE_H

#include "network/message.h"
namespace network {

    class PositionMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(PositionMessage)
        int32_t mActorGUID;
        double mX;
        double mY;
        double mOrientation;
        PositionMessage()
            : mActorGUID(0)
            , mX(0.0)
            , mY(0.0)
            , mOrientation(0.0)
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void PositionMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        ar & mX;
        ar & mY;
        ar & mOrientation;
    }

} // namespace network
#endif//INCLUDED_NETWORK_POSITION_MESSAGE_H
