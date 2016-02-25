#ifndef INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H

#include "component.h"
#include <boost/serialization/export.hpp>

class IStopOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IStopOnDeathComponent)
    virtual void SetStopped(bool stopped)=0;
    virtual bool IsStopped()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IStopOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IStopOnDeathComponent,"i_stop_on_death_component");
#endif//INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H
