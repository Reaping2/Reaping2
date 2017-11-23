#ifndef INCLUDED_CORE_CHEST_COMPONENT_H
#define INCLUDED_CORE_CHEST_COMPONENT_H

#include "i_chest_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include "price.h"

class ChestComponent : public IChestComponent
{
public:
    ChestComponent();
    virtual void SetPickupProfile( int32_t pickupProfile );
    virtual int32_t GetPickupProfile() const;
    virtual void SetPrice( Price price );
    virtual Price GetPrice() const;
    void InitFromPickupProfile( int32_t profileId );
    virtual void SetPickupDesc( core::PickupDesc const& pickupDesc );
    virtual core::PickupDesc const& GetPickupDesc() const;
protected:
    friend class ComponentFactory;
    int32_t mPickupProfile;
    Price mPrice;
    core::PickupDesc mPickupDesc;
public:
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ChestComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IChestComponent>(*this);
    ar& mPickupProfile;
    ar& mPrice;
    ar& mPickupDesc;
}

class ChestComponentLoader : public ComponentLoader<ChestComponent>
{
    virtual void BindValues();
protected:
    ChestComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( ChestComponent, ChestComponent, "chest_component" );

#endif//INCLUDED_CORE_CHEST_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "chest_component" -m "int32_t-pickupProfile Price-price"
