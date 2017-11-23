#ifndef INCLUDED_CORE_I_SWITCH_COMPONENT_H
#define INCLUDED_CORE_I_SWITCH_COMPONENT_H

#include "component.h"
#include "switch_state.h"

class ISwitchComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ISwitchComponent)
    virtual void SetState(SwitchState::Type state)=0;
    virtual SwitchState::Type GetState()const=0;
    virtual void SetSecsToEnd(double secsToEnd)=0;
    virtual double GetSecsToEnd()const=0;
    virtual void SetSecsToEndMax( double secsToEndMax ) = 0;
    virtual double GetSecsToEndMax()const = 0;
    virtual void SetStateChanged( bool stateChanged ) = 0;
    virtual bool IsStateChanged()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ISwitchComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_SWITCH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "switch_component" -m "SwitchState::Type-state double-secsToEnd"
