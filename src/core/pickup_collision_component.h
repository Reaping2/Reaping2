#ifndef INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
#define INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "item.h"
#include <boost/serialization/export.hpp>

class PickupCollisionComponent : public CollisionComponent
{
public:
    virtual void SetPickupContent( int32_t PickupContent );
    virtual int32_t GetPickupContent() const;
    virtual void SetItemType( ItemType::Type itemType );
    virtual ItemType::Type GetItemType() const;
    virtual void Save(Json::Value& component);

protected:
    PickupCollisionComponent();
    friend class ComponentFactory;
    int32_t mPickupContent;
    ItemType::Type mItemType;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void PickupCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<CollisionComponent>(*this);
    ar & mPickupContent;
    ar & mItemType;
}

class PickupCollisionComponentLoader: public ComponentLoader<PickupCollisionComponent>
{
    virtual void BindValues();
public:
    PickupCollisionComponentLoader();
    friend class ComponentLoaderFactory;
protected:
    CollisionClass& mCollisionClass;
};


BOOST_CLASS_EXPORT_KEY2(PickupCollisionComponent,"pickup_collision_component");
#endif//INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
