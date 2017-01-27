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
    : mCollisionClassType(CollisionClass::No_Collision)
    , mRadius(0.0)
    , mClipScene(true)
    , mDynamicRadius(false)
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
    SetDynamicRadius( true );
}

bool CollisionComponent::IsDynamicRadius() const
{
    return mDynamicRadius;
}

void CollisionComponent::SetDynamicRadius( bool dyn )
{
    mDynamicRadius = dyn;
}

bool CollisionComponent::IsClipScene() const
{
    return mClipScene;
}

void CollisionComponent::SetClipScene(bool clipScene)
{
    mClipScene = clipScene;
}

void CollisionComponentLoader::BindValues()
{
    Bind( "radius", func_double( &CollisionComponent::SetRadius ) );
    std::string istr;
    CollisionClass& collisionClass=CollisionClass::Get();
    if( Json::GetStr( ( *mSetters )["class"], istr ) )
    {
        Bind<CollisionClass::Type>( &CollisionComponent::SetCollisionClass, collisionClass( AutoId( istr ) ) );
    }
    Bind("clip_scene", func_bool(&CollisionComponent::SetClipScene));
    Bind("dynamic_radius", func_bool(&CollisionComponent::SetDynamicRadius));

}

void CollisionComponentLoader::FillProperties( ComponentHolder& actor ) const
{
    bool dynradius = false;
    if( mSetters != nullptr )
    {
        Json::GetBool( (*mSetters)["dynamic_radius"], dynradius );
    }
    ComponentLoader<CollisionComponent>::FillProperties( actor );
    actor.Get<ICollisionComponent>()->SetDynamicRadius( dynradius );
}

CollisionComponentLoader::CollisionComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( CollisionComponent, CollisionComponent );
