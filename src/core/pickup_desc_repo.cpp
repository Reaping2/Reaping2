#include "platform/i_platform.h"
#include "pickup_desc_repo.h"
#include "platform/filesystem_utils.h"

using platform::AutoId;

namespace core {

PickupDescRepo::PickupDescRepo()
    : Repository<PickupDesc>( mDefault )
    , mDefault( -1, Json::Value() )
{
    fs_utils::for_each( "actors", ".pickup", [&]( Json::Value const& desc )
    {
        int32_t Idx = AutoId(desc["name"].asString());
        mElements.insert( Idx, new PickupDesc( Idx, desc ) );
    } );
}

PickupDesc::PickupDesc( int32_t id, Json::Value const& setters )
    : mPickupContent(id)
{
    std::string istr;
    if (Json::GetStr( setters["type"], istr ))
    {
         mType = ItemType::Get()( AutoId( istr ) );
    }
    mPrice.Load( setters["price"] );
}

} // namespace core

