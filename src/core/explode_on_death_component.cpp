#include "core/explode_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

ExplodeOnDeathComponent::ExplodeOnDeathComponent()
    : IExplodeOnDeathComponent()
{
}


void ExplodeOnDeathComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["explosion_projectile"], istr))
    {
        Bind<int32_t>(&ExplodeOnDeathComponent::SetExplosionProjectile,AutoId(istr));
    }
    Bind("count",func_int32_t(&ExplodeOnDeathComponent::SetCount));
    Bind("scatter",func_double(&ExplodeOnDeathComponent::SetExplosionScatter));
    if( Json::GetStr( (*mSetters)["distribution"], istr))
    {
        Bind<ExplodeDistributionType::Type>(&ExplodeOnDeathComponent::SetDistribution,mExplodeDistributionType(AutoId(istr)));
    }
    Bind("secs_to_end_variance",func_double(&ExplodeOnDeathComponent::SetSecsToEndVariance));
    Bind("position_variance",func_int32_t(&ExplodeOnDeathComponent::SetPositionVariance));
    Bind("speed_variance",func_double(&ExplodeOnDeathComponent::SetSpeedVariance));
    Bind("add_actor_radius",func_bool(&ExplodeOnDeathComponent::SetAddActorRadius));
}

ExplodeOnDeathComponentLoader::ExplodeOnDeathComponentLoader()
    : mExplodeDistributionType(ExplodeDistributionType::Get())
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT(ExplodeOnDeathComponent, ExplodeOnDeathComponent);
