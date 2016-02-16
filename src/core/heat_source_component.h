#ifndef INCLUDED_CORE_HEAT_SOURCE_COMPONENT_H
#define INCLUDED_CORE_HEAT_SOURCE_COMPONENT_H

#include "i_heat_source_component.h"
#include "core/property_loader.h"

class HeatSourceComponent : public IHeatSourceComponent
{
public:
    HeatSourceComponent();
protected:
    friend class ComponentFactory;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void HeatSourceComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IHeatSourceComponent>(*this);
}

class HeatSourceComponentLoader : public ComponentLoader<HeatSourceComponent>
{
    virtual void BindValues();
protected:
    HeatSourceComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_HEAT_SOURCE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "heat_source_component"
