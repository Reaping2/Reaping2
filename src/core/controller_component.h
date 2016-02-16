#ifndef INCLUDED_CORE_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_CONTROLLER_COMPONENT_H

#include "core/i_controller_component.h"
#include "core/actor.h"

class ControllerComponent : public IControllerComponent
{
public:
    ControllerComponent();
    virtual void SetEnabled( bool Enabled );
    virtual bool IsEnabled();
    virtual void Update( double Seconds );
protected:
    bool mEnabled;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ControllerComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IControllerComponent>(*this);
    ar & mEnabled;
}

#endif//INCLUDED_CORE_CONTROLLER_COMPONENT_H
