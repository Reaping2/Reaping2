#ifndef INCLUDED_CORE_OWNER_COMPONENT_H
#define INCLUDED_CORE_OWNER_COMPONENT_H

#include "i_owner_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class OwnerComponent : public IOwnerComponent
{
public:
    OwnerComponent();
    virtual void SetOwnerGUID( int32_t ownerGUID );
    virtual int32_t GetOwnerGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mOwnerGUID;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void OwnerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IOwnerComponent>( *this );
    ar& mOwnerGUID;
}

class OwnerComponentLoader : public ComponentLoader<OwnerComponent>
{
    virtual void BindValues();
protected:
    OwnerComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( OwnerComponent, OwnerComponent, "owner_component" );
#endif//INCLUDED_CORE_OWNER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "owner_component" -m "int32_t-ownerGUID"
