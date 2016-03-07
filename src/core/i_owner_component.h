#ifndef INCLUDED_CORE_I_OWNER_COMPONENT_H
#define INCLUDED_CORE_I_OWNER_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class IOwnerComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IOwnerComponent )
    virtual void SetOwnerGUID( int32_t ownerGUID ) = 0;
    virtual int32_t GetOwnerGUID()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IOwnerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IOwnerComponent, IOwnerComponent, "i_owner_component" );
#endif//INCLUDED_CORE_I_OWNER_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_owner_component" -m "int32_t-ownerGUID"
