#include "core/explode_on_hit_component.h"

ExplodeOnHitComponent::ExplodeOnHitComponent()
    : IExplodeOnHitComponent()
    , mDetonatorMaterial(-1)
    , mAddRadius(0.0)
    , mExploded(false)
{
}

void ExplodeOnHitComponent::SetDetonatorMaterial(int32_t detonatorMaterial)
{
    mDetonatorMaterial=detonatorMaterial;
}

int32_t ExplodeOnHitComponent::GetDetonatorMaterial()const
{
    return mDetonatorMaterial;
}

void ExplodeOnHitComponent::SetAddRadius(double addRadius)
{
    mAddRadius=addRadius;
}

double ExplodeOnHitComponent::GetAddRadius()const
{
    return mAddRadius;
}

void ExplodeOnHitComponent::SetExploded(bool exploded)
{
    mExploded=exploded;
}

bool ExplodeOnHitComponent::IsExploded()const
{
    return mExploded;
}

void ExplodeOnHitComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["explosion_projectile"], istr))
    {
        Bind<int32_t>(&ExplodeOnHitComponent::SetExplosionProjectile,AutoId(istr));
    }
    Bind("count",func_int32_t(&ExplodeOnHitComponent::SetCount));
    Bind("scatter",func_double(&ExplodeOnHitComponent::SetExplosionScatter));
    
    if( Json::GetStr( (*mSetters)["detonator_material"], istr))
    {
        Bind<int32_t>(&ExplodeOnHitComponent::SetDetonatorMaterial,AutoId(istr));
    }
    Bind("add_radius",func_int32_t(&ExplodeOnHitComponent::SetAddRadius));
    if( Json::GetStr( (*mSetters)["distribution"], istr))
    {
        Bind<ExplodeDistributionType::Type>(&ExplodeOnHitComponent::SetDistribution,mExplodeDistributionType(AutoId(istr)));
    }
    Bind("secs_to_end_variance",func_double(&ExplodeOnHitComponent::SetSecsToEndVariance));
    Bind("position_variance",func_int32_t(&ExplodeOnHitComponent::SetPositionVariance));
    Bind("speed_variance",func_double(&ExplodeOnHitComponent::SetSpeedVariance));

}

ExplodeOnHitComponentLoader::ExplodeOnHitComponentLoader()
    : mExplodeDistributionType(ExplodeDistributionType::Get())
{
}
