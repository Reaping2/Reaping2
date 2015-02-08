#ifndef INCLUDED_NETWORK_MESSAGE_ORDER_H
#define INCLUDED_NETWORK_MESSAGE_ORDER_H
#include "boost/serialization/export.hpp"
#include "network/my_name_message.h"
#include "network/client_id_message.h"
#include "network/lifecycle_message.h"
#include "create_actor_message.h"
#include "set_ownership_message.h"
#include "position_message.h"
#include "platform/auto_id.h"
#include "move_message.h"
#include "player_controller_message.h"


    struct message_order
    {
        message_order()
        {
            int32_t type;
            type=network::ClientIdMessage::GetType_static();
            type=network::MyNameMessage::GetType_static();
            type=network::LifecycleMessage::GetType_static();
            type=network::CreateActorMessage::GetType_static();
            type=network::SetOwnershipMessage::GetType_static();
            type=network::PositionMessage::GetType_static();
            type=network::MoveMessage::GetType_static();
            type=network::PlayerControllerMessage::GetType_static();

            type=platform::AutoId("player");
        }
    } _msg_order;

    BOOST_CLASS_EXPORT_GUID(network::MyNameMessage,"my_name")
    BOOST_CLASS_EXPORT_GUID(network::ClientIdMessage, "client_id")
    BOOST_CLASS_EXPORT_GUID(network::LifecycleMessage, "lifecycle")
    BOOST_CLASS_EXPORT_GUID(network::CreateActorMessage, "create_actor")
    BOOST_CLASS_EXPORT_GUID(network::SetOwnershipMessage, "set_ownership")
    BOOST_CLASS_EXPORT_GUID(network::PositionMessage, "position")
    BOOST_CLASS_EXPORT_GUID(network::MoveMessage, "move")
    BOOST_CLASS_EXPORT_GUID(network::PlayerControllerMessage, "player_c")


#endif//INCLUDED_NETWORK_MESSAGE_ORDER_H