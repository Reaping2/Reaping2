#include "collision_component.h"
#include "i_position_component.h"
#include "platform/auto_id.h"
#include "platform/event.h"
#include "core/scene.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

using platform::AutoId;
using platform::EventServer;


CollisionComponent::CollisionComponent()
    : mCollisionClassType( CollisionClass::No_Collision )
    , mRadius( 0.0 )
{

}

CollisionClass::Type CollisionComponent::GetCollisionClass() const
{
    return mCollisionClassType;
}

void CollisionComponent::SetCollisionClass( CollisionClass::Type CollisionClassType )
{
    mCollisionClassType = CollisionClassType;
}

double CollisionComponent::GetRadius() const
{
    return mRadius;
}

void CollisionComponent::SetRadius( double Radius )
{
    mRadius = Radius;
}

void CollisionComponentLoader::BindValues()
{
    Bind( "radius", func_double( &CollisionComponent::SetRadius ) );
    std::string istr;
    if( Json::GetStr( ( *mSetters )["class"], istr ) )
    {
        Bind<CollisionClass::Type>( &CollisionComponent::SetCollisionClass, mCollisionClass( AutoId( istr ) ) );
    }

}

CollisionComponentLoader::CollisionComponentLoader()
    : mCollisionClass( CollisionClass::Get() )
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( CollisionComponent, CollisionComponent );
