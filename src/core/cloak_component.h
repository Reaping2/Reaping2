#ifndef INCLUDED_CORE_CLOAK_COMPONENT_H
#define INCLUDED_CORE_CLOAK_COMPONENT_H

#include "i_cloak_component.h"
#include "core/property_loader.h"

class CloakComponent : public ICloakComponent
{
public:
    CloakComponent();
    virtual void SetActive(bool active);
    virtual bool IsActive()const;
protected:
    friend class ComponentFactory;
    bool mActive;
private:
};

class CloakComponentLoader : public ComponentLoader<CloakComponent>
{
    virtual void BindValues();
protected:
    CloakComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_CLOAK_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "cloak_component" -m "bool-active"
