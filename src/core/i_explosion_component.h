#ifndef INCLUDED_CORE_I_EXPLOSION_COMPONENT_H
#define INCLUDED_CORE_I_EXPLOSION_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class IExplosionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IExplosionComponent )
    virtual void SetStartRadius( double startRadius ) = 0;
    virtual double GetStartRadius()const = 0;
    virtual void SetMaxRadius( double maxRadius ) = 0;
    virtual double GetMaxRadius()const = 0;
    virtual void SetScaleSpeed( double scaleSpeed ) = 0;
    virtual double GetScaleSpeed()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IExplosionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IExplosionComponent, IExplosionComponent, "i_explosion_component" );
#endif//INCLUDED_CORE_I_EXPLOSION_COMPONENT_H
