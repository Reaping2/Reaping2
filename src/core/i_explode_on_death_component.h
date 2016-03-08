#ifndef INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

#include "component.h"
#include "i_explode.h"
#include "platform/export.h"

class IExplodeOnDeathComponent : public Component, public IExplode
{
public:
    IExplodeOnDeathComponent();
    DEFINE_COMPONENT_BASE( IExplodeOnDeathComponent )
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IExplodeOnDeathComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
    ar& boost::serialization::base_object<IExplode>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IExplodeOnDeathComponent, IExplodeOnDeathComponent, "i_explode_on_death_component" );
#endif//INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
