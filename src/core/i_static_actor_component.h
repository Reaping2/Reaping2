#ifndef INCLUDED_CORE_I_STATIC_ACTOR_COMPONENT_H
#define INCLUDED_CORE_I_STATIC_ACTOR_COMPONENT_H

#include "component.h"

class IStaticActorComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IStaticActorComponent)
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IStaticActorComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_STATIC_ACTOR_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "static_actor_component"
