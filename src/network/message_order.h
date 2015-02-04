#ifndef INCLUDED_NETWORK_MESSAGE_ORDER_H
#define INCLUDED_NETWORK_MESSAGE_ORDER_H
#include "boost/serialization/export.hpp"
#include "network/my_name_message.h"
#include "network/client_id_message.h"


    struct message_order
    {
        message_order()
        {
            int32_t type;
            type=network::ClientIdMessage::GetType_static();
            type=network::MyNameMessage::GetType_static();
        }
    } _msg_order;

    BOOST_CLASS_EXPORT(network::MyNameMessage)

    BOOST_CLASS_EXPORT_GUID(network::ClientIdMessage, "client_id")


#endif//INCLUDED_NETWORK_MESSAGE_ORDER_H