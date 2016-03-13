#ifndef INCLUDED_CORE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_COLLISION_COMPONENT_H
#include "core/i_collision_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class CollisionComponent : public ICollisionComponent
{
public:
    virtual CollisionClass::Type GetCollisionClass()const;
    virtual void SetCollisionClass( CollisionClass::Type CollisionClassType );
    virtual double GetRadius()const;
    virtual void SetRadius( double Radius );
    virtual bool IsClipScene()const;
    virtual void SetClipScene( bool clipScene );
protected:
    CollisionComponent();
    friend class ComponentFactory;
    CollisionClass::Type mCollisionClassType;
    double mRadius;
    bool mClipScene;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void CollisionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ICollisionComponent>( *this );
    ar& mCollisionClassType;
    ar& mRadius;
    ar& mClipScene;
}

class CollisionComponentLoader: public ComponentLoader<CollisionComponent>
{
    virtual void BindValues();
protected:
    friend class ComponentLoaderFactory;
    CollisionClass& mCollisionClass;
public:
    CollisionComponentLoader();
};


REAPING2_CLASS_EXPORT_KEY2( CollisionComponent, CollisionComponent, "collision_component" );
#endif//INCLUDED_CORE_COLLISION_COMPONENT_H
