#ifndef INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

#include "component.h"
#include "i_explode.h"

class IExplodeOnDeathComponent : public Component, public IExplode
{
public:
    DEFINE_COMPONENT_BASE(IExplodeOnDeathComponent)
};

#endif//INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
