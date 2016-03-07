#include "core/detonate_on_hit_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

DetonateOnHitComponent::DetonateOnHitComponent()
    : mMaterial( -1 )
    , mAddRadius( 0.0 )
    , mRemoveOnHit( true )
{
}

void DetonateOnHitComponent::SetMaterial( int32_t material )
{
    mMaterial = material;
}

int32_t DetonateOnHitComponent::GetMaterial()const
{
    return mMaterial;
}

void DetonateOnHitComponent::SetAddRadius( double addRadius )
{
    mAddRadius = addRadius;
}

double DetonateOnHitComponent::GetAddRadius()const
{
    return mAddRadius;
}

void DetonateOnHitComponent::SetRemoveOnHit( bool removeOnHit )
{
    mRemoveOnHit = removeOnHit;
}

bool DetonateOnHitComponent::IsRemoveOnHit()const
{
    return mRemoveOnHit;
}

void DetonateOnHitComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( ( *mSetters )["material"], istr ) )
    {
        Bind<int32_t>( &DetonateOnHitComponent::SetMaterial, AutoId( istr ) );
    }
    Bind( "add_radius", func_int32_t( &DetonateOnHitComponent::SetAddRadius ) );
    Bind( "remove_on_hit", func_bool( &DetonateOnHitComponent::SetRemoveOnHit ) );
}

DetonateOnHitComponentLoader::DetonateOnHitComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( DetonateOnHitComponent, DetonateOnHitComponent );
