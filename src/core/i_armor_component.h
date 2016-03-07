#ifndef INCLUDED_CORE_I_ARMOR_COMPONENT_H
#define INCLUDED_CORE_I_ARMOR_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class IArmorComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IArmorComponent )
    virtual void SetCurrentArmor( int32_t currentArmor ) = 0;
    virtual int32_t GetCurrentArmor()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IArmorComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IArmorComponent, IArmorComponent, "i_armor_component" );
#endif//INCLUDED_CORE_I_ARMOR_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_armor_component" -m "int32_t-currentArmor"
