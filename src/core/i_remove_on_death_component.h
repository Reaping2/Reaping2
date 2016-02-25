#ifndef INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
#include "component.h"
#include <boost/serialization/export.hpp>

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
    virtual double GetRemainingTime() const = 0;
    virtual void SetRemainingTime( double rt ) = 0;
};

template<class Archive>
void IRemoveOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H

BOOST_CLASS_EXPORT_KEY2(IRemoveOnDeathComponent,"i_remove_on_death_component");
#endif//INCLUDED_CORE_I_REMOVE_ON_DEATH_COMPONENT_H
