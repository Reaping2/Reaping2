#ifndef INCLUDED_CORE_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H

#include "i_remove_components_on_death_component.h"
#include "core/property_loader.h"
#include <boost/serialization/vector.hpp>

class RemoveComponentsOnDeathComponent : public IRemoveComponentsOnDeathComponent
{
public:
    RemoveComponentsOnDeathComponent();
    virtual void SetComponents( std::vector<int32_t> const& components );
    virtual std::vector<int32_t> const& GetComponents()const;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    friend class ::boost::serialization::access;
    friend class ComponentFactory;
    std::vector<int32_t> mComponents;
private:
};

template<class Archive>
void RemoveComponentsOnDeathComponent::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<IRemoveComponentsOnDeathComponent>( *this );
    ar& mComponents;
}

class RemoveComponentsOnDeathComponentLoader : public ComponentLoader<RemoveComponentsOnDeathComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( RemoveComponentsOnDeathComponentLoader )
private:
    virtual void BindValues();
protected:
    RemoveComponentsOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

BOOST_CLASS_EXPORT_KEY2( RemoveComponentsOnDeathComponent, "remove_components_on_death_component" )
#endif//INCLUDED_CORE_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H

//command:  "classgenerator" -g "component" -c "remove_components_on_death_component" -m "std::vector<int32_t>-components"
