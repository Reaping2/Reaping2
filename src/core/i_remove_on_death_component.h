#ifndef INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#include "component.h"

class IRemoveOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IRemoveOnDeathComponent)
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IRemoveOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
