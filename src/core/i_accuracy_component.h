#ifndef INCLUDED_CORE_I_ACCURACY_COMPONENT_H
#define INCLUDED_CORE_I_ACCURACY_COMPONENT_H

#include "component.h"
#include "buffable.h"

class IAccuracyComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAccuracyComponent)
    virtual Buffable<int32_t>& GetAccuracy()=0;
};

#endif//INCLUDED_CORE_I_ACCURACY_COMPONENT_H
