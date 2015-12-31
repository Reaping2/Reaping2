#ifndef INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

#include "i_explode_on_hit_component.h"
#include "core/property_loader.h"

class ExplodeOnHitComponent : public IExplodeOnHitComponent
{
public:
    ExplodeOnHitComponent();
    virtual void SetExplosionProjectile(int32_t explosionProjectile);
    virtual int32_t GetExplosionProjectile()const;
    virtual void SetCount(int32_t count);
    virtual int32_t GetCount()const;
    virtual void SetScatter(double scatter);
    virtual double GetScatter()const;
    virtual void SetDetonatorMaterial(int32_t detonatorMaterial);
    virtual int32_t GetDetonatorMaterial()const;
    virtual void SetAddRadius(double addRadius);
    virtual double GetAddRadius()const;
    virtual void SetExploded(bool exploded);
    virtual bool IsExploded()const;
protected:
    friend class ComponentFactory;
    int32_t mExplosionProjectile;
    int32_t mCount;
    double mScatter;
    int32_t mDetonatorMaterial;
    double mAddRadius;
    bool mExploded;
private:
};

class ExplodeOnHitComponentLoader : public ComponentLoader<ExplodeOnHitComponent>
{
    virtual void BindValues();
protected:
    ExplodeOnHitComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_hit_component" -m "int32_t-detonatorMaterial double-addRadius bool-exploded"
