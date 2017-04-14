#ifndef INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"
#include "core/item.h"
#include "core/item_type.h"
#include "platform/settings.h"

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
    Settings& mSettings = Settings::Get();
    int32_t mTextSize = 76;
    int32_t mTextY = 50;
    glm::vec4 mTextColor = glm::vec4( 1.0 );
public:
    DEFINE_SUB_SYSTEM_BASE( PickupCollisionSubSystem )
    PickupCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void Collide( Actor& actor, Actor& other );

    void PickItUp( Actor &actor, Actor &other );

};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
