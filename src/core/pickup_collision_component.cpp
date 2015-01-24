#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/pickup_collision_component.h"
#include "platform/i_platform.h"
#include "platform/auto_id.h"

using platform::AutoId;

PickupCollisionComponent::PickupCollisionComponent()
    : CollisionComponent()
    , mPickupContent( 0 )
{

}

void PickupCollisionComponent::SetPickupContent(int32_t PickupContent)
{
    mPickupContent=PickupContent;
}

int32_t PickupCollisionComponent::GetPickupContent() const
{
    return mPickupContent;
}

void PickupCollisionComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["content"], istr))
    {
        Bind<int32_t>(&PickupCollisionComponent::SetPickupContent,AutoId(istr));
    }
}

PickupCollisionComponentLoader::PickupCollisionComponentLoader()
    : mCollisionClass(CollisionClass::Get())
{
    SetBase<CollisionComponentLoader>();
}
