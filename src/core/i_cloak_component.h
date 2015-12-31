#ifndef INCLUDED_CORE_I_CLOAK_COMPONENT_H
#define INCLUDED_CORE_I_CLOAK_COMPONENT_H

#include "component.h"

class ICloakComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ICloakComponent)
    virtual void SetActive(bool active)=0;
    virtual bool IsActive()const=0;
};

#endif//INCLUDED_CORE_I_CLOAK_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_cloak_component" -m "bool-active"
