#ifndef INCLUDED_NETWORK_MESSAGE_ORDER_H
#define INCLUDED_NETWORK_MESSAGE_ORDER_H
namespace network {
bool instantiateMessageOrder();
namespace {
volatile bool forceInstantiate = instantiateMessageOrder();
}
}

#endif//INCLUDED_NETWORK_MESSAGE_ORDER_H
