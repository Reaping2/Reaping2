#ifndef INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include "shot_collision_component.h"

class BounceCollisionComponent : public ShotCollisionComponent
{
public:
    BounceCollisionComponent();
    virtual void SetSpeedLossPercent( double speedLossPercent );
    virtual double GetSpeedLossPercent()const;
    virtual void SetUseShotCollision( bool useShotCollision );
    virtual bool IsUseShotCollision()const;
    virtual void SetResetActorsCollidedOnBounce(bool resetActorsCollidedOnBounce);
    virtual bool IsResetActorsCollidedOnBounce()const;
protected:
    friend class ComponentFactory;
    double mSpeedLossPercent;
    bool mUseShotCollision;
    bool mResetActorsCollidedOnBounce;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void BounceCollisionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ShotCollisionComponent>( *this );
    ar& mSpeedLossPercent;
    ar& mUseShotCollision;
}

class BounceCollisionComponentLoader : public ComponentLoader<BounceCollisionComponent>
{
    virtual void BindValues();
protected:
    BounceCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( BounceCollisionComponent, BounceCollisionComponent, "bounce_collision_component" );
#endif//INCLUDED_CORE_BOUNCE_COLLISION_COMPONENT_H
