#ifndef INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

#include "i_explode_on_death_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ExplodeOnDeathComponent : public IExplodeOnDeathComponent
{
public:
    ExplodeOnDeathComponent();
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ExplodeOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IExplodeOnDeathComponent>(*this);
}

class ExplodeOnDeathComponentLoader : public ComponentLoader<ExplodeOnDeathComponent>
{
    virtual void BindValues();
protected:
    ExplodeOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
    ExplodeDistributionType& mExplodeDistributionType;
};


REAPING2_CLASS_EXPORT_KEY2(ExplodeOnDeathComponent, ExplodeOnDeathComponent,"explode_on_death_component");
#endif//INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
