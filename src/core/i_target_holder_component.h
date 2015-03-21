#ifndef INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H
#define INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H

#include "component.h"

class ITargetHolderComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ITargetHolderComponent)
    virtual void SetTarget(Actor* target)=0;
    virtual Actor* GetTarget()const=0;
};

#endif//INCLUDED_CORE_I_TARGET_HOLDER_COMPONENT_H
