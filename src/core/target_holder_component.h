#ifndef INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H
#define INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H

#include "i_target_holder_component.h"
#include "core/property_loader.h"

class TargetHolderComponent : public ITargetHolderComponent
{
public:
    TargetHolderComponent();
    virtual void SetTarget(Actor* target);
    virtual Actor* GetTarget()const;
protected:
    friend class ComponentFactory;
    Actor* mTarget;
private:
};

class TargetHolderComponentLoader : public ComponentLoader<TargetHolderComponent>
{
    virtual void BindValues();
protected:
    TargetHolderComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H
