#ifndef INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"
#include "core/item.h"

namespace engine {

struct PickupEvent : Event
{
    Opt<Actor> mActor;
    Item::ItemType mItemType;
    int32_t mItemId;
    PickupEvent(Opt<Actor> actor, Item::ItemType itemType, int32_t itemId)
        :mActor(actor),mItemType(itemType),mItemId(itemId){}
};

class PickupCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(PickupCollisionSubSystem)
    PickupCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
