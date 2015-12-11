#ifndef INCLUDED_CORE_OWNER_COMPONENT_H
#define INCLUDED_CORE_OWNER_COMPONENT_H

#include "i_owner_component.h"
#include "core/property_loader.h"

class OwnerComponent : public IOwnerComponent
{
public:
    OwnerComponent();
    virtual void SetOwnerGUID(int32_t ownerGUID);
    virtual int32_t GetOwnerGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mOwnerGUID;
private:
};

class OwnerComponentLoader : public ComponentLoader<OwnerComponent>
{
    virtual void BindValues();
protected:
    OwnerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_OWNER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "owner_component" -m "int32_t-ownerGUID"
