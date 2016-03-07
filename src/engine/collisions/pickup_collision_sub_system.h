#ifndef INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"
#include "core/item.h"
#include "core/item_type.h"

namespace engine {

struct PickupEvent : Event
{
    Opt<Actor> mActor;
    ItemType::Type mItemType;
    int32_t mItemId;
    PickupEvent( Opt<Actor> actor, ItemType::Type itemType, int32_t itemId )
        : mActor( actor ), mItemType( itemType ), mItemId( itemId ) {}
};

class PickupCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( PickupCollisionSubSystem )
    PickupCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void Collide( Actor& actor, Actor& other );
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
