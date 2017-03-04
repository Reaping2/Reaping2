#ifndef INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H
#define INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H

#include "network/message.h"
#include "core/program_state.h"
#include "platform/export.h"
namespace network {

class LifecycleMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( LifecycleMessage )
    enum State : int32_t
    {
        Start = 0,
        SoldierProperties,
        WaitingForHost,
        AlreadyConnected,
        ClientList,
        SelectLevel
    };
    State mState;
    core::GameModes::Type mGameMode;
    std::string mSelectedLevel;
    int32_t mClientId;
    LifecycleMessage()
        : mState( Start )
        , mGameMode()
        , mSelectedLevel()
        , mClientId( -1 )
    {}
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void LifecycleMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mState;
    ar& mGameMode;
    ar& mSelectedLevel;
    ar& mClientId;
}

} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__LifecycleMessage, network::LifecycleMessage, "lifecycle" );
#endif//INCLUDED_NETWORK_LIFECYCLE_MESSAGE_H
