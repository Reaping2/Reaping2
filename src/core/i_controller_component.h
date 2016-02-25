#ifndef INCLUDED_CORE_I_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_I_CONTROLLER_COMPONENT_H

#include "component.h"
#include <boost/serialization/export.hpp>

class Actor;
class IControllerComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IControllerComponent)
    virtual void Update( double Seconds )=0;
    virtual void SetEnabled( bool Enabled )=0;
    virtual bool IsEnabled()=0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IControllerComponent,"i_controller_component");
#endif//INCLUDED_CORE_I_CONTROLLER_COMPONENT_H
