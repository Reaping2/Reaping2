#ifndef INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"

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
};

class BounceCollisionComponentLoader : public ComponentLoader<BounceCollisionComponent>
{
    virtual void BindValues();
protected:
    BounceCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
