#include "core/chest_component.h"

ChestComponent::ChestComponent()
    : mPickupProfile(-1)
    , mPrice()
{
}

void ChestComponent::SetPickupProfile( int32_t pickupProfile )
{
    mPickupProfile = pickupProfile;
}

int32_t ChestComponent::GetPickupProfile() const
{
    return mPickupProfile;
}

void ChestComponent::SetPrice( Price price )
{
    mPrice = price;
}

Price ChestComponent::GetPrice() const
{
    return mPrice;
}



void ChestComponentLoader::BindValues()
{
    auto const& priceJson = ( *mSetters )["price"];
    if( priceJson.isObject() )
    {
        Price price;
        price.Load( priceJson );
        Bind<Price>( &ChestComponent::SetPrice, price );
    }
    Bind( "pickup_profile", func_int32_t( &ChestComponent::SetPickupProfile ) );
}

ChestComponentLoader::ChestComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( ChestComponent, ChestComponent );
