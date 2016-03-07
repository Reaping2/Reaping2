#include "core/explode_on_hit_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

ExplodeOnHitComponent::ExplodeOnHitComponent()
    : IExplodeOnHitComponent()
    , mDetonatorMaterial( -1 )
    , mAddActorRadius( 0.0 )
    , mExploded( false )
{
}

void ExplodeOnHitComponent::SetDetonatorMaterial( int32_t detonatorMaterial )
{
    mDetonatorMaterial = detonatorMaterial;
}

int32_t ExplodeOnHitComponent::GetDetonatorMaterial()const
{
    return mDetonatorMaterial;
}

void ExplodeOnHitComponent::SetAddActorRadius( double addRadius )
{
    mAddActorRadius = addRadius;
}

double ExplodeOnHitComponent::GetAddRadius()const
{
    return mAddActorRadius;
}

void ExplodeOnHitComponent::SetExploded( bool exploded )
{
    mExploded = exploded;
}

bool ExplodeOnHitComponent::IsExploded()const
{
    return mExploded;
}

void ExplodeOnHitComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( ( *mSetters )["explosion_projectile"], istr ) )
    {
        Bind<int32_t>( &ExplodeOnHitComponent::SetExplosionProjectile, AutoId( istr ) );
    }
    Bind( "count", func_int32_t( &ExplodeOnHitComponent::SetCount ) );
    Bind( "scatter", func_double( &ExplodeOnHitComponent::SetExplosionScatter ) );

    if( Json::GetStr( ( *mSetters )["detonator_material"], istr ) )
    {
        Bind<int32_t>( &ExplodeOnHitComponent::SetDetonatorMaterial, AutoId( istr ) );
    }
    if( Json::GetStr( ( *mSetters )["distribution"], istr ) )
    {
        Bind<ExplodeDistributionType::Type>( &ExplodeOnHitComponent::SetDistribution, mExplodeDistributionType( AutoId( istr ) ) );
    }
    Bind( "secs_to_end_variance", func_double( &ExplodeOnHitComponent::SetSecsToEndVariance ) );
    Bind( "position_variance", func_int32_t( &ExplodeOnHitComponent::SetPositionVariance ) );
    Bind( "speed_variance", func_double( &ExplodeOnHitComponent::SetSpeedVariance ) );
    Bind( "add_actor_radius", func_bool( &ExplodeOnHitComponent::SetAddActorRadius ) );
}

ExplodeOnHitComponentLoader::ExplodeOnHitComponentLoader()
    : mExplodeDistributionType( ExplodeDistributionType::Get() )
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( ExplodeOnHitComponent, ExplodeOnHitComponent );
