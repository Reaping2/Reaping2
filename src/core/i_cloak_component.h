#ifndef INCLUDED_CORE_I_CLOAK_COMPONENT_H
#define INCLUDED_CORE_I_CLOAK_COMPONENT_H

#include "component.h"

class ICloakComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( ICloakComponent )
    virtual void SetActive( bool active ) = 0;
    virtual bool IsActive()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ICloakComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}

REAPING2_CLASS_EXPORT_KEY2( ICloakComponent, ICloakComponent, "i_cloak_component" );

#endif//INCLUDED_CORE_I_CLOAK_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_cloak_component" -m "bool-active"
