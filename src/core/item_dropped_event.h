#ifndef INCLUDED_CORE_ITEM_DROPPED_EVENT_H
#define INCLUDED_CORE_ITEM_DROPPED_EVENT_H

#include "platform/event.h"

namespace core {

struct ItemDroppedEvent : public platform::Event
{
    Item& mItem;
    ItemDroppedEvent(Item& item)
        :mItem(item){}
};

} // namespace core

#endif//INCLUDED_CORE_ITEM_DROPPED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "item_dropped_event" -m "Item&-item" -n "core"
