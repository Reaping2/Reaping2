#ifndef INCLUDED_CORE_I_OWNER_COMPONENT_H
#define INCLUDED_CORE_I_OWNER_COMPONENT_H

#include "component.h"

class IOwnerComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IOwnerComponent)
    virtual void SetOwnerGUID(int32_t ownerGUID)=0;
    virtual int32_t GetOwnerGUID()const=0;
};

#endif//INCLUDED_CORE_I_OWNER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_owner_component" -m "int32_t-ownerGUID"
