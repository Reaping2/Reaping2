#ifndef INCLUDED_CORE_I_ATTRACTABLE_COMPONENT_H
#define INCLUDED_CORE_I_ATTRACTABLE_COMPONENT_H

#include "component.h"
#include "scripted_controller/turn_to_target_act.h"

class IAttractableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAttractableComponent)
    virtual void SetAttractType(int32_t type)=0;
    virtual int32_t GetAttractType()const=0;
    virtual void SetDeceleration(int32_t deceleration)=0;
    virtual int32_t GetDeceleration()const=0;
    virtual void SetTurnToTargetAct( scriptedcontroller::TurnToTargetAct turnToTargetAct)=0;
    virtual scriptedcontroller::TurnToTargetAct& GetTurnToTargetAct()=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IAttractableComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_ATTRACTABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attractable_component" -m "int32_t-type int32_t-deceleration IAct-turnToTargetAct"
