#ifndef INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H
#define INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H

#include "platform/event.h"

namespace engine {

struct ItemChangedEvent : public platform::Event
{
    int32_t mActorGUID;
    int32_t mNormalId;
    int32_t mWeaponId;
    ItemChangedEvent( int32_t actorGUID, int32_t normalId, int32_t weaponId )
        : mActorGUID( actorGUID ), mNormalId( normalId ), mWeaponId( weaponId ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_ITEM_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "item_changed_event" -m "int32_t-actorGUID int32_t-normalId int32_t-weaponId"
