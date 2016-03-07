#ifndef INCLUDED_CORE_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_CONTROLLER_COMPONENT_H

#include "core/i_controller_component.h"
#include "core/actor.h"
#include "platform/export.h"

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
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ControllerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IControllerComponent>( *this );
    ar& mEnabled;
}


REAPING2_CLASS_EXPORT_KEY2( ControllerComponent, ControllerComponent, "controller_component" );
#endif//INCLUDED_CORE_CONTROLLER_COMPONENT_H
