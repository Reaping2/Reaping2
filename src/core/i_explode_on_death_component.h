#ifndef INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

#include "component.h"

class IExplodeOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IExplodeOnDeathComponent)
    virtual void SetExplosionProjectile(int32_t explosionProjectile)=0;
    virtual int32_t GetExplosionProjectile()const=0;
    virtual void SetCount(int32_t count)=0;
    virtual int32_t GetCount()const=0;
    virtual void SetScatter(double scatter)=0;
    virtual double GetScatter()const=0;
};

#endif//INCLUDED_CORE_I_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
