#ifndef INCLUDED_CORE_I_ARMOR_COMPONENT_H
#define INCLUDED_CORE_I_ARMOR_COMPONENT_H

#include "component.h"

class IArmorComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IArmorComponent)
    virtual void SetCurrentArmor(int32_t currentArmor)=0;
    virtual int32_t GetCurrentArmor()const=0;
};

#endif//INCLUDED_CORE_I_ARMOR_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_armor_component" -m "int32_t-currentArmor"
