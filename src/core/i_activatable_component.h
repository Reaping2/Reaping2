#ifndef INCLUDED_CORE_I_ACTIVATABLE_COMPONENT_H
#define INCLUDED_CORE_I_ACTIVATABLE_COMPONENT_H

#include "component.h"

class IActivatableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IActivatableComponent)
    virtual void SetEnabled(bool enabled)=0;
    virtual bool IsEnabled()const=0;
    virtual void SetActivatorGUID(int32_t guid)=0;
    virtual int32_t GetActivatorGUID()const=0;
    virtual bool IsActivated()const=0;
    virtual void SetHighlighted(bool highlighted)=0;
    virtual bool IsHighlighted()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IActivatableComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_ACTIVATABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "activatable_component" -m "bool-enabled bool-activated bool-highlighted"
