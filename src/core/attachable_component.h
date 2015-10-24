#ifndef INCLUDED_CORE_ATTACHABLE_COMPONENT_H
#define INCLUDED_CORE_ATTACHABLE_COMPONENT_H

#include "i_attachable_component.h"
#include "core/property_loader.h"

namespace ctf {

class AttachableComponent : public IAttachableComponent
{
public:
    AttachableComponent();
    virtual void SetAttachedGUID(int32_t attachedGUID);
    virtual int32_t GetAttachedGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mAttachedGUID;
private:
};

class AttachableComponentLoader : public ComponentLoader<AttachableComponent>
{
    virtual void BindValues();
protected:
    AttachableComponentLoader();
    friend class ComponentLoaderFactory;
};

} // namespace ctf

#endif//INCLUDED_CORE_ATTACHABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attachable_component" -m "int32_t-attachedGUID"
