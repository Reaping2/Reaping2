#ifndef INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H
#define INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H

#include "platform/event.h"
#include "core/item_type.h"

namespace engine {

    struct ItemChangedEvent : public platform::Event
    {
        int32_t mActorGUID;
        ItemType::Type mType;
        int32_t mItemId;
        int32_t mPrevItemId;
        bool mDropPrevItem;
        ItemChangedEvent( int32_t actorGUID, ItemType::Type type, int32_t itemId, int32_t prevItemId = 0, bool dropPrevItem = false )
            :mActorGUID( actorGUID ), mType( type ), mItemId( itemId ), mPrevItemId( prevItemId ), mDropPrevItem(dropPrevItem) {}
    };


} // namespace engine

#endif//INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "item_changed_event" -m "int32_t-actorGUID int32_t-normalId int32_t-weaponId"
