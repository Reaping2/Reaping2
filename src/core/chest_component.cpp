#include "core/chest_component.h"
#include "pickup_profiles_repo.h"
#include "pickup_desc_repo.h"

ChestComponent::ChestComponent()
    : mPickupProfile(-1)
    , mPrice()
    , mPickupDesc()
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



void ChestComponent::InitFromPickupProfile( int32_t profileId )
{
    static auto& mProfileRepo( core::PickupProfilesRepo::Get() );
    auto& profile( mProfileRepo( profileId ) );

    auto const& item = profile.Roll();

    static auto& mPickupDescRepo( core::PickupDescRepo::Get() );
    mPickupDesc = mPickupDescRepo( item.mPickupId );
    mPickupDesc.mPrice.Clear();
}

void ChestComponent::SetPickupDesc( core::PickupDesc const& pickupDesc )
{
    mPickupDesc = pickupDesc;
}

core::PickupDesc const& ChestComponent::GetPickupDesc() const
{
    return mPickupDesc;
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
    std::string istr;
    if (Json::GetStr( ( *mSetters )["pickup_profile"], istr ))
    {
        Bind<int32_t>( &ChestComponent::InitFromPickupProfile, AutoId( istr ) );
    }

}

ChestComponentLoader::ChestComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( ChestComponent, ChestComponent );
