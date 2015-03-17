#ifndef INCLUDED_CORE_EXPLOSION_COMPONENT_H
#define INCLUDED_CORE_EXPLOSION_COMPONENT_H

#include "i_explosion_component.h"
#include "core/property_loader.h"

class ExplosionComponent : public IExplosionComponent
{
public:
    virtual void SetStartRadius(double startRadius);
    virtual double GetStartRadius()const;
    virtual void SetMaxRadius(double maxRadius);
    virtual double GetMaxRadius()const;
    virtual void SetScaleSpeed(double scaleSpeed);
    virtual double GetScaleSpeed()const;
protected:
    ExplosionComponent();
    friend class ComponentFactory;
    double mStartRadius;
    double mMaxRadius;
    double mScaleSpeed;
private:
};

class ExplosionComponentLoader : public ComponentLoader<ExplosionComponent>
{
    virtual void BindValues();
    ExplosionComponentLoader();
protected:
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EXPLOSION_COMPONENT_H

