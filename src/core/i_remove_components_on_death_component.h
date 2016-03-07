#ifndef INCLUDED_CORE_I_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class IRemoveComponentsOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IRemoveComponentsOnDeathComponent )
    virtual void SetComponents( std::vector<int32_t> const& components ) = 0;
    virtual std::vector<int32_t> const& GetComponents()const = 0;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IRemoveComponentsOnDeathComponent::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Component>( *this );
}

REAPING2_CLASS_EXPORT_KEY2( IRemoveComponentsOnDeathComponent, IRemoveComponentsOnDeathComponent, "i_remove_components_on_death_component" )

#endif//INCLUDED_CORE_I_REMOVE_COMPONENTS_ON_DEATH_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_remove_components_on_death_component" -m "std::vector<int32_t>-components"
