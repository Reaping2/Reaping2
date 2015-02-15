#ifndef INCLUDED_NETWORK_MOVE_MESSAGE_H
#define INCLUDED_NETWORK_MOVE_MESSAGE_H

#include "network/message.h"
namespace network {

    class MoveMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(MoveMessage)
        int32_t mActorGUID;
        //double mHeading;
        int32_t mHeadingModifier;
        int32_t mSpeed;
        //double mSpeedX;
        //double mSpeedY;
        MoveMessage()
            : mActorGUID(0)
          //  , mHeading(0.0)
            , mHeadingModifier(0)
            , mSpeed(0)
            //, mSpeedX(0.0)
            //, mSpeedY(0.0)
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
        bool operator==(MoveMessage const& other)
        {
            return mActorGUID==other.mActorGUID
            //    && mHeading==other.mHeading
                && mHeadingModifier==other.mHeadingModifier
                && mSpeed==other.mSpeed;
                //&& mSpeedX==other.mSpeedX
                //&& mSpeedY==other.mSpeedY;
        }
    };

    template<class Archive>
    void MoveMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        //ar & mHeading;
        ar & mHeadingModifier;
        ar & mSpeed;
        //ar & mSpeedX;
        //ar & mSpeedY;
    }

} // namespace network
#endif//INCLUDED_NETWORK_MOVE_MESSAGE_H
