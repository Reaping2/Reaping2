#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/pickup_collision_component.h"
#include "platform/i_platform.h"
#include "platform/auto_id.h"
#include "item_type.h"
#include "platform/id_storage.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "pickup_profiles_repo.h"
#include "pickup_desc_repo.h"

using platform::AutoId;

PickupCollisionComponent::PickupCollisionComponent()
    : CollisionComponent()
    , mPickupContent( 0 )
    , mItemType( ItemType::Weapon )
    , mAutoPrice( false )
    , mPickupOnCollision( false )
{

}

void PickupCollisionComponent::SetPickupContent( int32_t PickupContent )
{
    mPickupContent = PickupContent;
}

int32_t PickupCollisionComponent::GetPickupContent() const
{
    return mPickupContent;
}

void PickupCollisionComponent::SetItemType( ItemType::Type itemType )
{
    mItemType = itemType;
}

ItemType::Type PickupCollisionComponent::GetItemType() const
{
    return mItemType;
}

void PickupCollisionComponent::Save( Json::Value& component )
{
    Component::Save( component );
    Json::Value SettersArr( Json::arrayValue );
    Json::Value Setters( Json::objectValue );
    std::string typeName;
    if ( platform::IdStorage::Get().GetName( ItemType::Get()( mItemType ), typeName ) )
    {
        Setters["type"] = Json::Value( typeName );
    }
    std::string pickupName;
    if ( platform::IdStorage::Get().GetName( mPickupContent, pickupName ) )
    {
        Setters["content"] = Json::Value( pickupName );
    }
    SettersArr.append( Setters );
    component["set"] = SettersArr;
}

void PickupCollisionComponent::SetPrice( Price price )
{
    mPrice = price;
}

Price& PickupCollisionComponent::GetPrice()
{
    return mPrice;
}

void PickupCollisionComponent::SetAutoPrice( bool autoPrice )
{
    mAutoPrice = autoPrice;
    if ( mAutoPrice&&mPickupContent > 0)
    {
        static auto& mPickupDescRepo( core::PickupDescRepo::Get() );
        auto const pickupDesc = mPickupDescRepo( mPickupContent );
        mPrice = pickupDesc.mPrice;
    }
}

bool PickupCollisionComponent::IsAutoPrice() const
{
    return mAutoPrice;
}

void PickupCollisionComponent::SetPickupOnCollision( bool pickup )
{
    mPickupOnCollision = pickup;
}

bool PickupCollisionComponent::IsPickupOnCollision() const
{
    return mPickupOnCollision;
}

void PickupCollisionComponent::InitFromPickupProfile( int32_t profieId )
{
    static auto& mProfileRepo(core::PickupProfilesRepo::Get());
    auto& profile(mProfileRepo( profieId ));

    auto const& item = profile.Roll();
	
	static auto& mPickupDescRepo(core::PickupDescRepo::Get());
	auto const pickupDesc = mPickupDescRepo(item.mPickupId);

    if (mAutoPrice)
    {
        mPrice = pickupDesc.mPrice;
    }
    mItemType = pickupDesc.mType;
    mPickupContent = pickupDesc.mPickupContent;
}

void PickupCollisionComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( ( *mSetters )["content"], istr ) )
    {
        Bind<int32_t>( &PickupCollisionComponent::SetPickupContent, AutoId( istr ) );
    }
    if( Json::GetStr( ( *mSetters )["type"], istr ) )
    {
        Bind<ItemType::Type>( &PickupCollisionComponent::SetItemType, ItemType::Get()( AutoId( istr ) ) );
    }
    auto const& priceJson = ( *mSetters )["price"];
    if( priceJson.isObject() )
    {
        Price price;
        price.Load( priceJson );
        Bind<Price>( &PickupCollisionComponent::SetPrice, price );
    }
    if (Json::GetStr( (*mSetters)["pickup_profile"], istr ))
    {
        Bind<int32_t>( &PickupCollisionComponent::InitFromPickupProfile, AutoId( istr ) );
    }

    Bind( "auto_price", func_bool( &PickupCollisionComponent::SetAutoPrice ) );

    Bind( "pickup_on_collision", func_bool( &PickupCollisionComponent::SetPickupOnCollision ) );
}

PickupCollisionComponentLoader::PickupCollisionComponentLoader()
    : mCollisionClass( CollisionClass::Get() )
{
    SetBase<CollisionComponentLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( PickupCollisionComponent, PickupCollisionComponent );
