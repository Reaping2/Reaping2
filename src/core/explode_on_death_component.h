#ifndef INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

#include "i_explode_on_death_component.h"
#include "core/property_loader.h"

class ExplodeOnDeathComponent : public IExplodeOnDeathComponent
{
public:
    ExplodeOnDeathComponent();
    virtual void SetExplosionProjectile(int32_t explosionProjectile);
    virtual int32_t GetExplosionProjectile()const;
    virtual void SetCount(int32_t count);
    virtual int32_t GetCount()const;
    virtual void SetScatter(double scatter);
    virtual double GetScatter()const;
protected:
    friend class ComponentFactory;
    int32_t mExplosionProjectile;
    int32_t mCount;
    double mScatter;
private:
};

class ExplodeOnDeathComponentLoader : public ComponentLoader<ExplodeOnDeathComponent>
{
    virtual void BindValues();
protected:
    ExplodeOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
