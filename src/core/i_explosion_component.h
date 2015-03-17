#ifndef INCLUDED_CORE_I_EXPLOSION_COMPONENT_H
#define INCLUDED_CORE_I_EXPLOSION_COMPONENT_H

#include "component.h"

class IExplosionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IExplosionComponent)
    virtual void SetStartRadius(double startRadius)=0;
    virtual double GetStartRadius()const=0;
    virtual void SetMaxRadius(double maxRadius)=0;
    virtual double GetMaxRadius()const=0;
    virtual void SetScaleSpeed(double scaleSpeed)=0;
    virtual double GetScaleSpeed()const=0;
};

#endif//INCLUDED_CORE_I_EXPLOSION_COMPONENT_H
