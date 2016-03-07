#include "platform/i_platform.h"
#include "core/shotgun.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

Shotgun::Shotgun( int32_t Id )
    : Weapon( Id )
    , IExplode()
{
}

Shotgun::Shotgun()
    : Weapon( -1 )
    , IExplode()
{
}

void ShotgunLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( ( *mSetters )["explosion_projectile"], istr ) )
    {
        Bind<int32_t>( &Shotgun::SetExplosionProjectile, AutoId( istr ) );
    }
    Bind( "count", func_int32_t( &Shotgun::SetCount ) );
    Bind( "scatter", func_double( &Shotgun::SetExplosionScatter ) );
    if( Json::GetStr( ( *mSetters )["distribution"], istr ) )
    {
        Bind<ExplodeDistributionType::Type>( &Shotgun::SetDistribution, mExplodeDistributionType( AutoId( istr ) ) );
    }
    Bind( "secs_to_end_variance", func_double( &Shotgun::SetSecsToEndVariance ) );
    Bind( "position_variance", func_int32_t( &Shotgun::SetPositionVariance ) );
    Bind( "speed_variance", func_double( &Shotgun::SetSpeedVariance ) );
    Bind( "add_actor_radius", func_bool( &Shotgun::SetAddActorRadius ) );
}

ShotgunLoader::ShotgunLoader()
    : mExplodeDistributionType( ExplodeDistributionType::Get() )
{
    SetBase<WeaponLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( Shotgun, Shotgun );
