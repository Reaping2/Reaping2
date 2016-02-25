#ifndef INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class BounceCollisionComponent : public CollisionComponent
{
public:
    BounceCollisionComponent();
    virtual void SetSpeedLossPercent(double speedLossPercent);
    virtual double GetSpeedLossPercent()const;
protected:
    friend class ComponentFactory;
    double mSpeedLossPercent;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void BounceCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<CollisionComponent>(*this);
    ar & mSpeedLossPercent;
}

class BounceCollisionComponentLoader : public ComponentLoader<BounceCollisionComponent>
{
    virtual void BindValues();
protected:
    BounceCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(BounceCollisionComponent,"bounce_collision_component");
#endif//INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
