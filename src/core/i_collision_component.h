#ifndef INCLUDED_CORE_I_COLLISION_COMPONENT_H
#define INCLUDED_CORE_I_COLLISION_COMPONENT_H
#include "component.h"
#include "core/collision_class.h"
#include "platform/export.h"

class ICollisionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( ICollisionComponent )
    virtual CollisionClass::Type GetCollisionClass()const = 0;
    virtual void SetCollisionClass( CollisionClass::Type CollisionClassType ) = 0;
    virtual double GetRadius()const = 0;
    virtual void SetRadius( double Radius ) = 0;
    virtual bool IsClipScene()const = 0;
    virtual void SetClipScene( bool clipScene ) = 0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ICollisionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( ICollisionComponent, ICollisionComponent, "i_collision_component" );
#endif//INCLUDED_CORE_I_COLLISION_COMPONENT_H