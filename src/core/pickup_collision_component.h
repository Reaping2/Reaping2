#ifndef INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
#define INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "item.h"

class PickupCollisionComponent : public CollisionComponent
{
public:
    virtual void SetPickupContent( int32_t PickupContent );
    virtual int32_t GetPickupContent() const;
    virtual void SetItemType( ItemType::Type itemType );
    virtual ItemType::Type GetItemType() const;
protected:
    PickupCollisionComponent();
    friend class ComponentFactory;
    int32_t mPickupContent;
    ItemType::Type mItemType;
};

class PickupCollisionComponentLoader: public ComponentLoader<PickupCollisionComponent>
{
    virtual void BindValues();
public:
    PickupCollisionComponentLoader();
    friend class ComponentLoaderFactory;
protected:
    CollisionClass& mCollisionClass;
};

#endif//INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
