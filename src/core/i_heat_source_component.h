#ifndef INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H
#define INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H

#include "component.h"

class IHeatSourceComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IHeatSourceComponent)
};

#endif//INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_heat_source_component"
