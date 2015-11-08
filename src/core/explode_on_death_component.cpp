#include "core/explode_on_death_component.h"

ExplodeOnDeathComponent::ExplodeOnDeathComponent()
    : mExplosionProjectile(-1)
    , mCount(1)
    , mScatter(0.0)
{
}

void ExplodeOnDeathComponent::SetExplosionProjectile(int32_t explosionProjectile)
{
    mExplosionProjectile=explosionProjectile;
}

int32_t ExplodeOnDeathComponent::GetExplosionProjectile()const
{
    return mExplosionProjectile;
}

void ExplodeOnDeathComponent::SetCount(int32_t count)
{
    mCount=count;
}

int32_t ExplodeOnDeathComponent::GetCount()const
{
    return mCount;
}

void ExplodeOnDeathComponent::SetScatter(double scatter)
{
    mScatter=scatter;
}

double ExplodeOnDeathComponent::GetScatter()const
{
    return mScatter;
}

void ExplodeOnDeathComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["explosion_projectile"], istr))
    {
        Bind<int32_t>(&ExplodeOnDeathComponent::SetExplosionProjectile,AutoId(istr));
    }
    Bind("count",func_int32_t(&ExplodeOnDeathComponent::SetCount));
    Bind("scatter",func_double(&ExplodeOnDeathComponent::SetScatter));
}

ExplodeOnDeathComponentLoader::ExplodeOnDeathComponentLoader()
{
}
