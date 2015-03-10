#ifndef INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

#include "core/i_explode_on_death_component.h"
#include "core/property_loader.h"

class ExplodeOnDeathComponent : public IExplodeOnDeathComponent
{
public:
    ExplodeOnDeathComponent();
    virtual void SetExplosionProjectile( int32_t explosionProjectile );
    virtual int32_t GetExplosionProjectile() const;
protected:
    int32_t mExplosionProjectile;
};

class ExplodeOnDeathComponentLoader: public ComponentLoader<ExplodeOnDeathComponent>
{
    virtual void BindValues();
public:
    ExplodeOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H