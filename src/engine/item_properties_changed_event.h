#ifndef INCLUDED_ENGINE_ITEM_PROPERTIES_CHANGED_EVENT_H
#define INCLUDED_ENGINE_ITEM_PROPERTIES_CHANGED_EVENT_H

#include "platform/event.h"
#include "core/item.h"

namespace engine {

struct ItemPropertiesChangedEvent : public platform::Event
{
    Item const& mItem;
    ItemPropertiesChangedEvent(Item const& item)
        :mItem(item){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEM_PROPERTIES_CHANGED_EVENT_H

//command:  "../../../Reaping2_build/build/tools/classgenerator/classgenerator" -g "event" -c "item_properties_changed_event" -m "Item-mItem" -n "engine"
