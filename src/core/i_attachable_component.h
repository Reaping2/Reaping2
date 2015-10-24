#ifndef INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H
#define INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H

#include "component.h"
namespace ctf {

class IAttachableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAttachableComponent)
    virtual void SetAttachedGUID(int32_t attachedGUID)=0;
    virtual int32_t GetAttachedGUID()const=0;
};

} // namespace ctf

#endif//INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_attachable_component" -m "int32_t-attachedGUID"
