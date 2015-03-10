#include "platform/i_platform.h"
#include "core/explode_on_death_component.h"
#include "core/i_health_component.h"
#include "core/actor.h"
#include <boost/assert.hpp>

ExplodeOnDeathComponent::ExplodeOnDeathComponent()
{

}

void ExplodeOnDeathComponent::SetExplosionProjectile(int32_t explosionProjectile)
{
    mExplosionProjectile=explosionProjectile;
}

int32_t ExplodeOnDeathComponent::GetExplosionProjectile() const
{
    return mExplosionProjectile;
}

ExplodeOnDeathComponentLoader::ExplodeOnDeathComponentLoader()
{

}

void ExplodeOnDeathComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["explosion_projectile"], istr))
    {
        Bind<int32_t>(&ExplodeOnDeathComponent::SetExplosionProjectile,AutoId(istr));
    }
}
