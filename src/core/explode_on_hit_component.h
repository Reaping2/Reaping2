#ifndef INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

#include "i_explode_on_hit_component.h"
#include "core/property_loader.h"

class ExplodeOnHitComponent : public IExplodeOnHitComponent
{
public:
    ExplodeOnHitComponent();
    virtual void SetDetonatorMaterial(int32_t detonatorMaterial);
    virtual int32_t GetDetonatorMaterial()const;
    virtual void SetAddRadius(double addRadius);
    virtual double GetAddRadius()const;
    virtual void SetExploded(bool exploded);
    virtual bool IsExploded()const;
protected:
    friend class ComponentFactory;
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
    ExplodeDistributionType& mExplodeDistributionType;
};

#endif//INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_hit_component" -m "int32_t-detonatorMaterial double-addRadius bool-exploded"
