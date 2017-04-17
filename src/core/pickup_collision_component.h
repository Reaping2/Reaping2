#ifndef INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
#define INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "item.h"
#include "platform/export.h"
#include "price.h"

class PickupCollisionComponent : public CollisionComponent
{
public:
    virtual void SetPickupContent( int32_t PickupContent );
    virtual int32_t GetPickupContent() const;
    virtual void SetItemType( ItemType::Type itemType );
    virtual ItemType::Type GetItemType() const;
    virtual void Save( Json::Value& component );
    virtual void SetPrice( Price price );
    virtual Price& GetPrice();
    virtual void InitFromPickupProfile( int32_t profieId );
protected:
    PickupCollisionComponent();
    friend class ComponentFactory;
    int32_t mPickupContent;
    ItemType::Type mItemType;
    Price mPrice;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PickupCollisionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<CollisionComponent>( *this );
    ar& mPickupContent;
    ar& mItemType;
    ar& mPrice;
}

class PickupCollisionComponentLoader: public ComponentLoader<PickupCollisionComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( PickupCollisionComponentLoader )
private:
    virtual void BindValues();
public:
    PickupCollisionComponentLoader();
    friend class ComponentLoaderFactory;
protected:
    CollisionClass& mCollisionClass;
};


REAPING2_CLASS_EXPORT_KEY2( PickupCollisionComponent, PickupCollisionComponent, "pickup_collision_component" );
#endif//INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
