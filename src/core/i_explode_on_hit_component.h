#ifndef INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H

#include "component.h"
#include "i_explode.h"

class IExplodeOnHitComponent : public Component, public IExplode
{
public:
    DEFINE_COMPONENT_BASE(IExplodeOnHitComponent)
    virtual void SetDetonatorMaterial(int32_t detonatorMaterial)=0;
    virtual int32_t GetDetonatorMaterial()const=0;
    virtual void SetAddRadius(double addRadius)=0;
    virtual double GetAddRadius()const=0;
    virtual void SetExploded(bool exploded)=0;
    virtual bool IsExploded()const=0;
};

#endif//INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_explode_on_hit_component" -m "int32_t-detonatorMaterial double-addRadius bool-exploded"
