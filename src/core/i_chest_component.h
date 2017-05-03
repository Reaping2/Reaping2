#ifndef INCLUDED_CORE_I_CHEST_COMPONENT_H
#define INCLUDED_CORE_I_CHEST_COMPONENT_H

#include "component.h"
#include "price.h"

class IChestComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IChestComponent)
    virtual void SetPickupProfile(int32_t pickupProfile)=0;
    virtual int32_t GetPickupProfile()const=0;
    virtual void SetPrice(Price price)=0;
    virtual Price GetPrice()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IChestComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_CHEST_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "chest_component" -m "int32_t-pickupProfile Price-price"
