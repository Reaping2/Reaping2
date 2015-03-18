#ifndef INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H

#include "component.h"

class IStopOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IStopOnDeathComponent)
    virtual void SetStopped(bool stopped)=0;
    virtual bool IsStopped()const=0;
};

#endif//INCLUDED_CORE_I_STOP_ON_DEATH_COMPONENT_H
